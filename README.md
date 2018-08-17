# Multiple-Producer-Single-Consumer Queue Benchmarks

This repository contains a set of benchmarks for evaluating different
implementations of multiple-producer-single-consumer queues, for use as a ready
queue in a threading package.

Due to the specific use case, the latency benchmark examines the distribution
of the latency from immediately before enqueueing to the time the same element
is dequeued from the queue.

This latency benchmark does nothing to verify that the queue behaves correctly
for multiple producers and a single consumer.

The throughput benchmark (not yet implemented) assumes that the queue behaves
correctly under multiple producers and attempts to measure the maximum
throughput of the queue as the number of successful dequeues from the single
consumer each second.

The current set of MPSC queues that are benchmarked are listed below.
 - Intrusive non-blocking queue from [samanbarghi](https://github.com/samanbarghi/MPSCQ).
 - Intrusive blocking queue from [samanbarghi](https://github.com/samanbarghi/MPSCQ).
 - Non-Intrusive blocking queue from [samanbarghi](https://github.com/samanbarghi/MPSCQ).
 - Wait-free queue from [dbittman](https://github.com/dbittman/waitfree-mpsc-queue)

This benchmark assumes that the queue repositories are cloned in the parent
directory of this repository.

## Dependencies

The only dependency beyond the queues being benchmarked is
[PlatformLab/PerfUtils](https://github.com/PlatformLab/PerfUtils/).  It should
be cloned into the parent directory and built before this benchmark is built.

## Adding a new queue

To add a new queue to the benchmark, it suffices to add a subclass of
`AbstractMPSCQ` which wraps the appropriate operations, and then add an
instantiation to the `descriptionAndQueue` data structure in
`src/EnqueueToDequeLatencyDist.cc`.
