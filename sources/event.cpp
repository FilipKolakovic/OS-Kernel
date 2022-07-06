#include "event.h"
#include "kernel.h"
#include "KernelEv.h"
#include <stdlib.h>

Event::Event (IVTNo ivtNo){
	lock();
	if(IVTEntry::getEntry(ivtNo) == NULL) myImpl = new KernelEv(ivtNo);
	else myImpl = NULL;
	unlock();
}

Event::~Event (){
	lock();
	delete myImpl;
	unlock();
}

//interrupt masked because in half of wait, signal can be called from interrupt and this is a critical section for event value
void Event::wait (){
	lock();
	myImpl->wait();
	unlock();
}

void Event::signal(){
	myImpl->signal();
}
