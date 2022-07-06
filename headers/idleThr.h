#ifndef _IDLETHR_H_
#define _IDLETHR_H_

#include "thread.h"

class Thread;
class PCB;

class IdleThread : public Thread {
public:
	IdleThread();
	~IdleThread();
	virtual void run();
	void start();

	PCB* getIdlePCB() const;
};

#endif
