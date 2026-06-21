#include "Scheduler.hpp"

Scheduler::Scheduler(std::vector<Process> procs, ISchedulingAlgorithm* algo)
    : processes(std::move(procs))
    , algorithm(algo)
    , current_time(0)
    , cpu_current(nullptr)
    , cpu_current_burst_remaining(0)
    , cpu_rr_quantum_counter(0)
    , rr_quantum(0)
    , is_rr(false)
    , is_srtn(false)
    , total_cpu_idle_time(0)
    , total_simulation_time(0)
    , visualizer(nullptr)
    , next_arrival_idx(0)
{
    is_rr = (dynamic_cast<RoundRobinScheduler*>(algorithm) != nullptr);
    is_srtn = (dynamic_cast<SRTNScheduler*>(algorithm) != nullptr);
    if (is_rr) {
        rr_quantum = static_cast<RoundRobinScheduler*>(algorithm)->quantum;
        cpu_rr_quantum_counter = rr_quantum;
    }
}

void Scheduler::setVisualizer(Visualizer* vis) {
    visualizer = vis;
}

void Scheduler::runSimulation() {
    while (true) {
        arrivals_this_tick.clear();
        r_returnees_this_tick.clear();

        // 1. New arrivals at current_time
        handleNewArrivals();

        // 2. Process CPU execution
        processCPU();

        // 3. Process Resource
        processResource();

        // 4. Conflict resolution: merge arrivals and R returnees into ready_queue
        resolveConflict();

        // 5. SRTN preemption check
        checkSRTNPreemption();

        // 6. Pick next CPU process if idle
        pickNextCPUProcess();

        // 7. Record Gantt for this tick
        recordGantt();

        // 8. Notify Visualizer
        notifyVisualizer();

        // 9. Add waiting time for processes in ready_queue
        for (auto* p : ready_queue) {
            p->addWaitingTime(1);
        }

        // 10. Check termination
        if (isSimulationFinished()) {
            break;
        }

        // 11. Advance time
        ++current_time;
    }

    total_simulation_time = current_time + 1;
}

void Scheduler::handleNewArrivals() {
    while (next_arrival_idx < processes.size() &&
           processes[next_arrival_idx].arrival_time == current_time) {
        Process* p = &processes[next_arrival_idx];
        arrivals_this_tick.push_back(p);
        ++next_arrival_idx;
    }
}

void Scheduler::processCPU() {
    if (cpu_current == nullptr) return;

    cpu_current_burst_remaining--;
    cpu_current->getCurrentBurst().remaining = cpu_current_burst_remaining;

    if (is_rr) {
        cpu_rr_quantum_counter--;
    }

    // Check if CPU burst completed
    if (cpu_current_burst_remaining <= 0) {
        cpu_current->advanceBurst();

        if (cpu_current->isFinished()) {
            cpu_current->markCompletion(current_time);
            cpu_current = nullptr;
        } else {
            // Next burst is RESOURCE → send to Resource
            resource.addRequest(cpu_current);
            cpu_current = nullptr;
        }

        if (is_rr) {
            cpu_rr_quantum_counter = rr_quantum;
        }
        return;
    }

    // RR preemption: quantum expired but burst not finished
    if (is_rr && cpu_rr_quantum_counter <= 0) {
        cpu_rr_quantum_counter = rr_quantum;
        ready_queue.push_back(cpu_current);
        cpu_current = nullptr;
    }
}

void Scheduler::processResource() {
    resource.tick();

    while (Process* p = resource.popReleased()) {
        if (p->isFinished()) {
            p->markCompletion(current_time + 1);
        } else {
            r_returnees_this_tick.push_back(p);
        }
    }
}

void Scheduler::resolveConflict() {
    // New arrivals go first, then R returnees go after
    for (auto* p : arrivals_this_tick) {
        p->enterReadyQueue(current_time);
        ready_queue.push_back(p);
    }
    for (auto* p : r_returnees_this_tick) {
        p->enterReadyQueue(current_time);
        ready_queue.push_back(p);
    }
}

void Scheduler::checkSRTNPreemption() {
    if (!is_srtn || cpu_current == nullptr) return;
    if (ready_queue.empty()) return;

    int current_remaining = cpu_current->getCurrentBurst().remaining;
    for (auto* p : ready_queue) {
        if (p->getRemainingCPUTotal() < current_remaining) {
            // Preempt current process
            ready_queue.push_back(cpu_current);
            cpu_current = nullptr;
            return;
        }
    }
}

void Scheduler::pickNextCPUProcess() {
    if (cpu_current != nullptr) return;
    if (ready_queue.empty()) return;

    int idx = algorithm->pickNext(ready_queue, current_time);
    if (idx < 0 || idx >= static_cast<int>(ready_queue.size())) return;

    cpu_current = ready_queue[idx];
    ready_queue.erase(ready_queue.begin() + idx);

    cpu_current_burst_remaining = cpu_current->getCurrentBurst().remaining;
    cpu_current->markFirstResponse(current_time);

    if (is_rr) {
        cpu_rr_quantum_counter = rr_quantum;
    }
}

void Scheduler::recordGantt() {
    cpu_gantt.push_back(cpu_current ? cpu_current->id : -1);

    if (cpu_current == nullptr) {
        ++total_cpu_idle_time;
    }
}

SystemSnapshot Scheduler::buildSnapshot() {
    SystemSnapshot snap;
    snap.current_time = current_time;
    snap.cpu_process = cpu_current;
    snap.cpu_remaining = cpu_current_burst_remaining;
    snap.cpu_burst_total = (cpu_current != nullptr)
        ? cpu_current->getCurrentBurst().duration : 0;
    snap.res_process = resource.current_user;
    snap.res_remaining = resource.remaining_time;
    snap.res_burst_total = (resource.current_user != nullptr)
        ? resource.current_user->getCurrentBurst().duration : 0;
    snap.ready_queue = ready_queue;
    snap.res_queue = resource.getQueueSnapshot();
    snap.algo_name = algo_name;
    snap.quantum = rr_quantum;
    return snap;
}

void Scheduler::notifyVisualizer() {
    if (visualizer != nullptr && visualizer->enabled) {
        visualizer->render(buildSnapshot());
    }
}

bool Scheduler::isSimulationFinished() {
    if (cpu_current != nullptr) return false;
    if (!ready_queue.empty()) return false;
    if (resource.is_busy) return false;
    if (resource.getQueueSize() > 0) return false;
    if (!resource.released_queue.empty()) return false;

    for (const auto& p : processes) {
        if (!p.isFinished()) return false;
    }

    return true;
}
