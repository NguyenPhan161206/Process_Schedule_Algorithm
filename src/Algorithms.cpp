#include "Algorithms.hpp"
#include <climits>

// --- FCFS ---
int FCFSScheduler::pickNext(std::deque<Process*>& ready_queue, int /*current_time*/) {
    if (ready_queue.empty()) return -1;
    return 0;
}

// --- Round Robin ---
RoundRobinScheduler::RoundRobinScheduler(int q)
    : quantum(q) {}

int RoundRobinScheduler::pickNext(std::deque<Process*>& ready_queue, int /*current_time*/) {
    if (ready_queue.empty()) return -1;
    return 0;
}

// --- SJF (non-preemptive) ---
int SJFScheduler::pickNext(std::deque<Process*>& ready_queue, int /*current_time*/) {
    if (ready_queue.empty()) return -1;
    int best_idx = 0;
    int best_remaining = ready_queue[0]->getRemainingCPUTotal();
    for (size_t i = 1; i < ready_queue.size(); ++i) {
        int rem = ready_queue[i]->getRemainingCPUTotal();
        if (rem < best_remaining) {
            best_remaining = rem;
            best_idx = static_cast<int>(i);
        }
    }
    return best_idx;
}

// --- SRTN (preemptive) ---
int SRTNScheduler::pickNext(std::deque<Process*>& ready_queue, int /*current_time*/) {
    if (ready_queue.empty()) return -1;
    int best_idx = 0;
    int best_remaining = ready_queue[0]->getRemainingCPUTotal();
    for (size_t i = 1; i < ready_queue.size(); ++i) {
        int rem = ready_queue[i]->getRemainingCPUTotal();
        if (rem < best_remaining) {
            best_remaining = rem;
            best_idx = static_cast<int>(i);
        }
    }
    return best_idx;
}
