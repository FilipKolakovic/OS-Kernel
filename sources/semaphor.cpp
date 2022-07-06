#include <stdlib.h>
#include "semaphor.h"
#include "kernelse.h"
#include "kernel.h"
#include "schedule.h"



Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

int Semaphore::wait(int toBlock) {
	lock();
	int v=0;
	v = myImpl->wait(toBlock);
	unlock();
	return v;
}

void Semaphore::signal() {
	lock();
	myImpl->signal();
	unlock();
}

int Semaphore::val() const {
	return myImpl->val();
}
