#include "pcb.h"
#include "schedule.h"
#include "kernel.h"
#include "IdleThr.h"
#include "semaphor.h"
#include "queue.h"
#include <dos.h>
#include <iostream.h>


class Kernel;
class Scheduler;

Thread::Thread(StackSize stackSize, Time pcbTimeSlice) {
	lock();
	myPCB = new PCB(this, stackSize, pcbTimeSlice);
	unlock();
}

void Thread::start(){
	lock();
	myPCB->setState(READY);
	myPCB->createStack();
	Scheduler::put(myPCB);
	unlock();
}

Thread::~Thread() {
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

void Thread::waitToComplete(){
	lock();
    if(myPCB == NULL ||  myPCB == (PCB*)Kernel::running  || this == Kernel::idle ||  myPCB->getState() == FINISHED || myPCB->getState() == NEW  || this == Kernel::starting){
		unlock();
		return;
	}

	Kernel::running->setState(BLOCKED);
	myPCB->getPendingQueue()->put((PCB*)Kernel::running);
	unlock();
	Kernel::dispatch();

}

void Thread::sleep(Time timeToSleep){
	lock();
	Kernel::running->setState(BLOCKED);
	Kernel::running->setTimeToSleep(timeToSleep);
	Kernel::sleepQueue->put((PCB*)Kernel::running);
	Kernel::dispatch();
	unlock();
}

void dispatch() {
	lock();
	Kernel::dispatch();
	unlock();
}











