#ifndef _OS1_KEYBOARD_EVENT_
#define _OS1_KEYBOARD_EVENT_

#include <thread.h>

class BoundedBuffer;



class KeyboardEvent :public Thread{
public:

	KeyboardEvent (BoundedBuffer*);  
	virtual ~KeyboardEvent () {waitToComplete();}
protected:

	virtual void run();

	char znak;

private:
  
	BoundedBuffer* myBuffer;
};

#endif
