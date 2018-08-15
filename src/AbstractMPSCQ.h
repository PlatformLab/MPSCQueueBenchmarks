#ifndef ABSTRACT_MPSCQ_H
#define  ABSTRACT_MPSCQ_H
/**
 * A simple queue interface to make all the different queues look the same to
 * the benchmarking program.
 */
class AbstractMPSCQ {
  public:
    virtual bool enqueue(void* item) = 0;
    virtual bool dequeue(void** item) = 0;
};
#endif
