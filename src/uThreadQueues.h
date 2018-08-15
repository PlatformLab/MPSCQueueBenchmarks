#ifndef  UTHREAD_QUEUES_H
#define UTHREAD_QUEUES_H

#include "AbstractMPSCQ.h"
#include "MPSCQueue.h"

class Dummy: public MPSCQueue<Dummy>::Node { };
/**
  * Elements here must inherit from Node.
  */
class IntrusiveNonBlockingQueue : public AbstractMPSCQ {
  public:
    virtual bool enqueue(void* item) {
        internalQueue.push(*reinterpret_cast<Dummy*>(item));
        return true;
    }

    virtual bool dequeue(void** item) {
        *item = internalQueue.pop();
        return *item != nullptr;
    }

  private:
    MPSCQueue<Dummy> internalQueue;
};
#endif
