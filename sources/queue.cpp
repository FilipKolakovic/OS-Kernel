#include "pcb.h"
#include "queue.h"
#include "kernel.h"
#include "schedule.h"
#include "kernelse.h"


Queue::Queue() {
	lock();
	first = NULL;
	last = NULL;
	size= 0;
	unlock();
}

Queue::Elem::Elem(PCB* newpcb,Elem* nxt) {
	lock();
	pcb = newpcb;
	next = nxt;
	unlock();
}


Queue::~Queue() {
	lock();
	while(first != NULL){
		Elem* old = first;
		first = first->next;
		delete old;
	}
	size = 0;
	last = NULL;
	unlock();
}

int Queue::getSize() const {
	return size;
}

int Queue::isEmpty() const {
	return first == NULL;
}


PCB* Queue::getFirstPCB() const {
	if( first == NULL ) return NULL;
	else
		return first->pcb;
}

PCB* Queue::removeFirstPCB() {
	lock();
	if(first == NULL){ last = NULL; unlock(); return NULL; }
	else {
		Elem *temp = first;
		first = first->next;
		size --;
		unlock();
		return temp->pcb;
	}
}

void Queue::removePCB(PCB* delPCB) {
	lock();
	Elem* curr = first;
	Elem* prev = NULL;
	while(curr != NULL && (curr->pcb != delPCB)){
		prev = curr;
		curr = curr->next;
	}

	if(curr == NULL) {unlock(); return;}

	if(prev == NULL) first = curr->next;
	else prev->next = curr->next;

	if(last == curr) last = prev;

	delete curr;

	size --;
	unlock();
}

void Queue::put(PCB* pcb) {
	lock();
	Elem* el = new Elem(pcb);
	if(first == NULL) first = el;
	else last->next = el;
	last = el;
	size++;
	unlock();
}

void Queue::decTime(){
	lock();
	Elem *tek = first;
	Elem *pret = NULL;
	while(tek) {
		tek->pcb->timeToSleep-- ;
		if(tek->pcb->timeToSleep > 0){
			pret = tek;
			tek = tek->next;
		}
		else{
			if(pret == NULL) first = tek->next;
			else pret->next = tek->next;

			if(last == tek) last = pret;

			Elem *pcbToDelete = tek;

			tek->pcb->setState(READY);
			Scheduler::put(tek->pcb);

			delete pcbToDelete;
			size--;
		}
	}

	unlock();
}
