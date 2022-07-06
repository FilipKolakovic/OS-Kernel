#ifndef _PCB_H_
#define _PCB_H_
#include <stdlib.h>

#include "thread.h"
#include "semaphor.h"

class Queue;
class KernelSem;

typedef enum {NEW, READY, RUNNING, BLOCKED, FINISHED} State;


class PCB {
public:
	PCB(Thread*, StackSize, Time);
	~PCB();

	Queue* getPendingQueue() const;
	void freePendingQueue();
	void createStack();
	StackSize getStackSize() const;
	Time getTimeSlice() const;
	State getState() const;
	Thread* getThread() const;
	ID getId() const;
	void setState(State);
	void setThread(Thread*);
	void setTimeToSleep(Time);

private:
	static void wrapper();
	static ID threadNum;

	ID threadId;
	Thread *thread;
	Time pcbTimeSlice;
	Time timeLeft;
	Time timeToSleep;
	StackSize stackSize;
	unsigned int* stack;
	unsigned int sp, ss, bp;
    State state;
	Queue* pendingQueue;

	friend class Kernel;
	friend class Queue;
	friend class KernelSem;
};

#endif
