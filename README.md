# Multiple-Producer-Single-Consumer Queue Benchmarks

This repository contains a set of benchmarks for evaluating different
implementations of multiple-producer-single-consumer queues, for use as a ready
queue in a threading package.

Due to the specific use case, the latency benchmark examines the distribution
of the latency from immediately before enqueueing to the time the same element
is dequeued from the queue.

This latency benchmark does nothing to verify that the queue behaves correctly
for multiple producers and a single consumer.

The throughput benchmark assumes that the queue behaves correctly under
multiple producers and attempts to measure the maximum throughput of the queue
as the number of successful dequeues from the single consumer each second.
