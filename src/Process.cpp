#include "Process.hpp"

Process::Process(int _id, int _arrival)
    : id(_id)
    , arrival_time(_arrival)
    , current_burst_idx(0)
    , first_response_time(-1)
    , completion_time(-1)
    , total_waiting_time(0)
    , time_entered_ready_queue(-1)
{}

bool Process::isFinished() const {
    return current_burst_idx >= bursts.size();
}

TaskBurst& Process::getCurrentBurst() {
    return bursts[current_burst_idx];
}

void Process::advanceBurst() {
    if (current_burst_idx < bursts.size()) {
        ++current_burst_idx;
    }
}

int Process::getRemainingCPUTotal() const {
    int total = 0;
    for (size_t i = current_burst_idx; i < bursts.size(); ++i) {
        if (bursts[i].type == BurstType::CPU) {
            total += bursts[i].remaining;
        }
    }
    return total;
}

void Process::markFirstResponse(int time) {
    if (first_response_time == -1) {
        first_response_time = time;
    }
}

void Process::markCompletion(int time) {
    completion_time = time;
}

void Process::addWaitingTime(int additional_time) {
    total_waiting_time += additional_time;
}

void Process::enterReadyQueue(int current_time) {
    time_entered_ready_queue = current_time;
}
