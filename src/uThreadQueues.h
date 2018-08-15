#ifndef  UTHREAD_QUEUES_H
#define UTHREAD_QUEUES_H

#include "AbstractMPSCQ.h"
#include "MPSCQueue.h"

class Dummy: public MPSCQueue<Dummy>::Node {
  public:
    int value;
};
/**
  * Elements here must inherit from Node.
  */
class IntrusiveNonBlockingQueue : public AbstractMPSCQ {
  public:
    virtual bool enqueue(void* item) {
        Dummy* dummy = reinterpret_cast<Dummy*>(item);
        dummy->value = 7;
        internalQueue.push(*dummy);
        return true;
    }

    virtual bool dequeue(void** item) {
        *item = internalQueue.pop();
        Dummy* dummy = reinterpret_cast<Dummy*>(*item);
        if (dummy != nullptr && dummy->value != 7) {
            fprintf(stderr, "Data was not preserved! Exiting...");
            abort();
        }
        return *item != nullptr;
    }

  private:
    MPSCQueue<Dummy> internalQueue;
};
#endif
