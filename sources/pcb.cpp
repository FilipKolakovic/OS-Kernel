#include "pcb.h"
#include "schedule.h"
#include "kernel.h"
#include "idleThr.h"
#include "queue.h"
#include <dos.h>
#include <iostream.h>

class Kernel;

/*stackSize limits*/
const StackSize minStackSize = 256;
const StackSize maxStackSize = 65536; // 64KB


/*static fields*/
ID PCB::threadNum = 0;


PCB::PCB(Thread* thr, StackSize ssize, Time tslice) {
  	lock();
  	threadId = threadNum++;
  	thread = thr;
  	stackSize = ssize;
  	pcbTimeSlice = tslice;
  	timeLeft = tslice;
  	timeToSleep = 0;
  	state = NEW;
  	stack = NULL;
  	ss = 0;
  	sp = 0;
  	bp = 0;

  	pendingQueue = new Queue();
	unlock();
}

PCB::~PCB() {
	lock()
	delete pendingQueue;
	delete [] stack;
	if(thread != NULL)thread->myPCB = NULL;
	unlock();
}


void PCB::createStack(){
	if(stackSize < minStackSize) stackSize = minStackSize;
	if(stackSize > maxStackSize) stackSize = maxStackSize;

	stackSize=stackSize/2;
	stack = new unsigned int[stackSize];

	stack[stackSize - 2] = 0x200;
	stack[stackSize - 3] = FP_SEG(&(PCB::wrapper));
	stack[stackSize - 4] = FP_OFF(&(PCB::wrapper));

	stack[stackSize - 13] = FP_OFF(stack + stackSize - 1); // for fork, bp points to last position of stack

	ss = FP_SEG(stack + stackSize - 2 - 11);
	sp = FP_OFF(stack + stackSize - 2 - 11);

	bp = sp;
}


void PCB::freePendingQueue() {
	PCB* pcb;
	while((pcb = ( pendingQueue->removeFirstPCB() ) ) != NULL ){
		if(pcb->state != READY){
			pcb->state = READY;
			Scheduler::put(pcb);
		}
	}
}

void PCB::wrapper(){
	Kernel::running->thread->run();
	lock();
	Kernel::running->setState(FINISHED);
	Kernel::running->freePendingQueue();
	unlock();
	Kernel::dispatch();
}


State PCB::getState() const { return state; }

Thread* PCB::getThread() const { return thread; }

StackSize PCB::getStackSize() const { return stackSize; }

Time PCB::getTimeSlice() const { return pcbTimeSlice; }

ID PCB::getId() const { return threadId; }

Queue* PCB::getPendingQueue() const { return pendingQueue; }


void PCB::setThread(Thread* thr){ thread = thr; }

void PCB::setState(State state){ this->state = state; }

void PCB::setTimeToSleep(Time tslp){this->timeToSleep = tslp;}





