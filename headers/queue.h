
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include<stdlib.h>

#include "pcb.h"

class PCB;

class Queue{

	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB*,Elem* nxt = NULL);
	};

	Elem* first,*last;
	int size;
public:
	Queue();
	~Queue();

	void put(PCB* pcb);

	PCB* getFirstPCB() const;

	PCB* removeFirstPCB();
	void removePCB(PCB*);

	int getSize() const;
	int isEmpty() const;

	void decTime();

};


#endif
