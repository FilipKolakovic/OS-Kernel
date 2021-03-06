#ifndef _event_h_
#define _event_h_

#include "IVTEntry.h"

//declaration of interrupt routine #numEntry
//creating IVTEntry object on stack linked with that entryNum and that routine
//interrupt routine for #numEntry

#define PREPAREENTRY(numEntry, callOld)\
\
void interrupt inter##numEntry(...); \
\
IVTEntry newEntry##numEntry(numEntry, &inter##numEntry); \
\
void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if(callOld == 1) newEntry##numEntry.callOldRoutine();\
	dispatch();\
}

class Event{
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};

#endif
