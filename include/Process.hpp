#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <cstddef>

enum class BurstType { CPU, RESOURCE };

struct TaskBurst {
    BurstType type;
    int duration;
    int remaining;

    TaskBurst(BurstType t, int d)
        : type(t), duration(d), remaining(d) {}
};

class Process {
public:
    int id;
    int arrival_time;
    std::vector<TaskBurst> bursts;
    size_t current_burst_idx;
    int first_response_time;
    int completion_time;
    int total_waiting_time;
    int time_entered_ready_queue;

    Process(int _id, int _arrival);

    bool isFinished() const;
    TaskBurst& getCurrentBurst();
    void advanceBurst();
    int getRemainingCPUTotal() const;
    void markFirstResponse(int time);
    void markCompletion(int time);
    void addWaitingTime(int additional_time);
    void enterReadyQueue(int current_time);
};

#endif
