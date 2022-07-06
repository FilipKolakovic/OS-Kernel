#ifndef _THREAD_H_
#define _THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time x 55ms
const Time defaultTimeSlice = 2; // 110 ms
typedef int ID;

class PCB;

class Thread {

public:
  void start();
  void waitToComplete();
  virtual ~Thread();

 static void sleep(Time timeToSleep);

protected:
  friend class PCB;
  friend class Kernel;
  friend class IdleThread;

  Thread (StackSize stackSize = defaultStackSize, Time pcbTimeSlice = defaultTimeSlice);
  virtual void run() {}

private:
  PCB* myPCB;

};

void dispatch();

#endif
