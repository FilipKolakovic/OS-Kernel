#include "kernelse.h"
#include "pcb.h"
#include "schedule.h"
#include "queue.h"
#include <iostream.h>

KernelSem::KernelSem(int init) {
	value = init;
}

int KernelSem::wait(int toBlock) {
	 lock();
	 int ret = 0;
	 if (!toBlock && value<=0)
		 ret = -1;
	 else
		 if (--value<0) {
	 		ret = 1;
	 		KernelSem::block();
	 	}
	 unlock();
	 return ret;
}

void KernelSem::signal() {
	value++;
	if( value <= 0 )
	{
		PCB* t = waitQueue.removeFirstPCB();
		t->setState(READY);
		Scheduler::put(t);
	}
}

int KernelSem::val() const { return value; }

void KernelSem::block(){
	Kernel::running->setState(BLOCKED);
	waitQueue.put((PCB*)Kernel::running);
	Kernel::dispatch();
}

