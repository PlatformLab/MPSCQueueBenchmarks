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
#include <thread>
#include "AbstractMPSCQ.h"
#include "uThreadQueues.h"
#include "WaitFreeQueue.h"
#include "PerfUtils/Perf.h"
#include "PerfUtils/Cycles.h"

#define NUM_ITERATIONS 100000
#define NUM_DISTINCT_CONTEXT 50

using PerfUtils::Cycles;

AbstractMPSCQ* queue;

struct Task {
    char dummy[64];
};

// Use multiple tasks so that it's not always the same task getting enqueued /
// dequeued.
Task tasks[NUM_DISTINCT_CONTEXT];
thread_local int taskIndex = 0;

// We use the two separate variables consumerRecordedTime and endTime,
// separated by a cache-line sized pad, to avoid overheads from consumer trying
// to write while the producer is trying to read.
std::atomic<bool> consumerRecordedTime;
char pad[64];
std::atomic<uint64_t> endTime;

// Consumer is long-running, mimicing a kernel thread polling for user-space
// threads on a ready queue.
void consumer() {
    consumerRecordedTime = true;
    void* output;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        while (!queue->dequeue(&output));
        endTime = Cycles::rdtsc();
        consumerRecordedTime = true;
    }
}

// Producer is called multiple times by the main benchmark thread.
void producer(uint64_t* output) {
   consumerRecordedTime = false;
   uint64_t startTime = Cycles::rdtsc();

   // Keep trying until enqueue succeeds.
   while (!queue->enqueue(tasks + taskIndex));

   // Wait for the consumer to record time.
   while (!consumerRecordedTime);
   *output = endTime - startTime;

   // Use a different task on the next round
   taskIndex++;
   if (taskIndex == NUM_DISTINCT_CONTEXT) taskIndex = 0;
}

// This function is necessary because Cycles::toNanoseconds uses a default
// valued parameter, which doesn't manifest when it is passed through a
// function pointer.
uint64_t cyclesToNanoseconds(uint64_t cycles) {
    return Cycles::toNanoseconds(cycles);
}

int main(){
    // Benchmark each one
    std::pair<const char*, AbstractMPSCQ*> descriptionAndQueue[] = {
        {"uThread_IntrusiveNonBlocking", new IntrusiveNonBlockingQueue},
        {"uThread_IntrusiveBlocking", new IntrusiveBlockingQueue},
        {"uThread_NonIntrusiveBlocking", new BlockingQueue},
        {"dbittman_WaitFreeQueue", new WaitFreeQueue(100)}
    };

    for (auto pair: descriptionAndQueue) {
        const char* desc = pair.first;
        queue = pair.second;

        // Start the consumer
        consumerRecordedTime = false;
        std::thread dequeuer(consumer);

        // Wait for consumer to fully start
        while (!consumerRecordedTime);

        Statistics stats = PerfUtils::manualBench(producer, NUM_ITERATIONS);
        stats = transformStatistics(stats, cyclesToNanoseconds);
        printStatistics(stats, desc);
        dequeuer.join();
    }
}
