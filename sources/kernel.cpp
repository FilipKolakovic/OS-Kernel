#include "kernel.h"
#include "schedule.h"
#include "pcb.h"
#include "idleThr.h"
#include "kernelse.h"
#include "queue.h"
#include <iostream.h>
#include <dos.h>
#include <stdlib.h>


volatile char Kernel::contextChangeRequired = 0;
volatile PCB* Kernel::running = NULL;
Thread* Kernel::starting = NULL;
IdleThread* Kernel::idle = NULL;
pInterrupt Kernel::oldRoutine = NULL;

Queue* Kernel::sleepQueue = new Queue();

unsigned int tss, tsp, tbp; // global variables for stack segment, stack pointer and base pointer
static volatile PCB *newThread;

void tick();

void interrupt Kernel::Timer(...) {

	if(!contextChangeRequired) {
		tick();
		asm int 60h
		sleepQueue->decTime();
	}
	asm { cli };

	if (!contextChangeRequired && running->pcbTimeSlice != 0 && running->timeLeft > 0) running->timeLeft--;
	if (!contextChangeRequired && (running->timeLeft > 0 || running->pcbTimeSlice == 0) ) return;
	contextChangeRequired = 0;
	if( running->state == READY && running != idle->myPCB )
	Scheduler::put((PCB*)running);

		newThread = Scheduler::get();
		if (newThread == NULL) newThread = idle->myPCB;
		asm {
			mov tsp, sp
			mov tss, ss
		}

		running->sp = tsp;
		running->ss = tss;
		running = newThread;
		tsp = running->sp;
		tss = running->ss;

		asm {
			mov sp, tsp
			mov ss, tss
		}


		running->timeLeft = running->pcbTimeSlice;
	return;
}

void Kernel::startUp(){
	lock();
	oldRoutine = getvect(8);
	setvect(8, &Timer);
	setvect(0x60, oldRoutine);
	starting = new Thread(0x10000, 1);
	starting->myPCB->state = READY;
	running = (volatile PCB*)starting->myPCB;
	idle = new IdleThread();
	idle->start();
	unlock();
}

void Kernel::terminate(){
	if( (PCB*)running != starting->myPCB ) return;
	lock();
	setvect(8,oldRoutine);
	delete starting;
	delete idle;
	unlock();
}

void Kernel::dispatch() {
	lock();
	contextChangeRequired = 1;
	Timer();
	contextChangeRequired = 0;
	unlock();
}

IdleThread* Kernel::getIdleThread() { return idle; }

PCB* Kernel::getRunningThread(){ return (PCB*)running;}
