#ifndef MPSCQ_CPP
#define MPSCQ_CPP

extern "C" {
/**
 * This file is a cpp compatible header with for the mpscq library.
 */
struct mpscq;

/* create a new mpscq. If n == NULL, it will allocate
 * a new one and return it. If n != NULL, it will
 * initialize the structure that was passed in.·
 * capacity must be greater than 1, and it is recommended
 * to be much, much larger than that. */
struct mpscq *mpscq_create(struct mpscq *n, size_t capacity);

/* enqueue an item into the queue. Returns true on success
 * and false on failure (queue full). This is safe to call
 * from multiple threads */
bool mpscq_enqueue(struct mpscq *q, void *obj);

/* dequeue an item from the queue and return it.
 * THIS IS NOT SAFE TO CALL FROM MULTIPLE THREADS.
 * Returns NULL on failure, and the item it dequeued
 * on success */
void *mpscq_dequeue(struct mpscq *q);

/* get the number of items in the queue currently */
size_t mpscq_count(struct mpscq *q);

/* get the capacity of the queue */
size_t mpscq_capacity(struct mpscq *q);

/* destroy a mpscq. Frees the internal buffer, and
 * frees q if it was created by passing NULL to mpscq_create */
void mpscq_destroy(struct mpscq *q);

}

#endif
