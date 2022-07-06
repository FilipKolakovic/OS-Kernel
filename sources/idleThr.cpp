#include "pcb.h"
#include "idleThr.h"
#include "kernel.h"

IdleThread::IdleThread() : Thread(256,1){}
IdleThread::~IdleThread() { delete myPCB; }

void IdleThread::run() {
	while (1);
}

void IdleThread::start() {
	lock();
	myPCB->setState(READY);
	myPCB->createStack();
	unlock();
}

PCB* IdleThread::getIdlePCB() const{ return myPCB; }




