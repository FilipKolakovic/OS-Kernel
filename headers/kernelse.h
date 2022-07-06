#ifndef _KERNELSE_H_
#define _KERNELSE_H_

#include "queue.h"
#include "semaphor.h"
#include "kernel.h"

class KernelSem {
public:
	friend class Semaphore;

	KernelSem(int init=1);

	int wait(int);
	void signal();

	void block();

	int val() const;


private:
	int value;
	Queue waitQueue;
};

#endif
