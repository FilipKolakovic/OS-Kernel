#ifndef KERNEL_H_
#define KERNEL_H_

#include "pcb.h"
#include "thread.h"
#include "idlethr.h"
#include "event.h"

typedef void interrupt (*pInterrupt)(...);


#define lock() {asm{pushf; cli;}}

#define unlock() {asm{popf}}

class Kernel {

public:
	static void startUp();
	static void terminate();
	static void interrupt Timer(...); //interrupt routine
	static void dispatch();

	static IdleThread* getIdleThread();

	static Queue *sleepQueue;

	static PCB* getRunningThread();


private:
	friend class Thread;
	friend class PCB;
	friend class IdleThread;
	friend class KernelSem;
	//friend class KernelEv;

	static Thread* starting;
	static volatile PCB* running;
	static volatile char contextChangeRequired;
	static IdleThread* idle;

	static pInterrupt oldRoutine;
};

#endif
