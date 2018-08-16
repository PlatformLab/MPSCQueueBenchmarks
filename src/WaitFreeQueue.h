#ifndef WAIT_FREE_QUEUE_H
#define WAIT_FREE_QUEUE_H

#include "AbstractMPSCQ.h"
#include "mpscq_cpp.h"

/**
 * A wrapper around Daniel Bittman's implementation of an MPSCQ.
 */
class WaitFreeQueue : public AbstractMPSCQ {
  public:
    virtual bool enqueue(void* item) {
        return mpscq_enqueue(internalQueue, item);
    }

    virtual bool dequeue(void** item) {
        *item = mpscq_dequeue(internalQueue);
        return *item != NULL;
    }

    WaitFreeQueue(size_t capacity) {
        internalQueue = mpscq_create(NULL, capacity);
    }
    ~WaitFreeQueue() {
        mpscq_destroy(internalQueue);
    }

  private:
    struct mpscq* internalQueue;
};

#endif
