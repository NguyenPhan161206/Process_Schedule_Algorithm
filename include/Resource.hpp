#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Process.hpp"
#include <queue>
#include <vector>

class Resource {
public:
    Process* current_user;
    int remaining_time;
    bool is_busy;
    std::queue<Process*> res_queue;
    std::queue<Process*> released_queue;
    std::vector<int> gantt_history;
    int total_idle_time;

    Resource();

    void addRequest(Process* p);
    void tick();
    Process* popReleased();
    int getQueueSize() const;
    std::queue<Process*> getQueueSnapshot() const;
};

#endif
