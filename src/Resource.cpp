#include "Resource.hpp"

Resource::Resource()
    : current_user(nullptr)
    , remaining_time(0)
    , is_busy(false)
    , total_idle_time(0)
{}

void Resource::addRequest(Process* p) {
    res_queue.push(p);
}

void Resource::tick() {
    // Record gantt and idle based on state at START of this tick
    gantt_history.push_back(is_busy ? current_user->id : -1);
    if (!is_busy) ++total_idle_time;

    if (is_busy) {
        --remaining_time;
        --current_user->getCurrentBurst().remaining;

        if (remaining_time <= 0) {
            current_user->advanceBurst();
            released_queue.push(current_user);
            current_user = nullptr;
            is_busy = false;
        }
    }

    if (!is_busy && !res_queue.empty()) {
        current_user = res_queue.front();
        res_queue.pop();
        remaining_time = current_user->getCurrentBurst().duration;
        current_user->getCurrentBurst().remaining = remaining_time;
        is_busy = true;
    }
}

Process* Resource::popReleased() {
    if (released_queue.empty()) return nullptr;
    Process* p = released_queue.front();
    released_queue.pop();
    return p;
}

int Resource::getQueueSize() const {
    return static_cast<int>(res_queue.size());
}

std::queue<Process*> Resource::getQueueSnapshot() const {
    return res_queue;
}
