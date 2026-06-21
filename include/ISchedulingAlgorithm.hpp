#ifndef ISCHEDULINGALGORITHM_HPP
#define ISCHEDULINGALGORITHM_HPP

#include "Process.hpp"
#include <deque>
#include <vector>

class ISchedulingAlgorithm {
public:
    virtual ~ISchedulingAlgorithm() = default;
    virtual int pickNext(std::deque<Process*>& ready_queue, int current_time) = 0;
};

#endif
