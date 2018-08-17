/* Copyright (c) 2018 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
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

class BlockingQueue : public AbstractMPSCQ {
  private:
    class Dummy {};
  public:
    typedef NonIntrusiveBlockingMPSCQueue<Dummy>::Node Node;
    virtual bool enqueue(void* item) {
        if (internalQueue.push(*reinterpret_cast<Node*>(item))) {
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
    NonIntrusiveBlockingMPSCQueue<Dummy> internalQueue;
    semaphore sem;
};
#endif
