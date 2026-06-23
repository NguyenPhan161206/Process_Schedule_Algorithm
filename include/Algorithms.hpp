#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

// Khai báo thuật toán và kế thừa thuật toán

#include "ISchedulingAlgorithm.hpp"

class FCFSScheduler : public ISchedulingAlgorithm {
public:
    int pickNext(std::deque<Process*>& ready_queue, int current_time) override;
};

class RoundRobinScheduler : public ISchedulingAlgorithm {
public:
    int quantum;

    explicit RoundRobinScheduler(int q = 2);
    int pickNext(std::deque<Process*>& ready_queue, int current_time) override;
};

class SJFScheduler : public ISchedulingAlgorithm {
public:
    int pickNext(std::deque<Process*>& ready_queue, int current_time) override;
};

class SRTNScheduler : public ISchedulingAlgorithm {
public:
    int pickNext(std::deque<Process*>& ready_queue, int current_time) override;
};

#endif
