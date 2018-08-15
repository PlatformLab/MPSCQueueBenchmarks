#ifndef UTHREAD_QUEUES_H
#define UTHREAD_QUEUES_H

#include "AbstractMPSCQ.h"
#include "MPSCQueue.h"
#include "Semaphore.h"

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
    class Dummy: public MPSCQueue<Dummy>::Node { };
    MPSCQueue<Dummy> internalQueue;
};

class IntrusiveBlockingQueue : public AbstractMPSCQ {
  public:
    virtual bool enqueue(void* item) {
        if (internalQueue.push(*reinterpret_cast<Dummy*>(item))) {
            sem.post();
        }
        return true;
    }

    virtual bool dequeue(void** item) {
        *item = internalQueue.pop();
        if (*item == nullptr) {
            sem.wait();
            return false;
        }
        return true;
    }
  private:
    class Dummy: public BlockingMPSCQueue<Dummy>::Node { };
    BlockingMPSCQueue<Dummy> internalQueue;
    semaphore sem;
};
#endif
