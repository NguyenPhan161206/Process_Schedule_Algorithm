#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Process.hpp"
#include "ISchedulingAlgorithm.hpp"
#include "Resource.hpp"
#include "Algorithms.hpp"
#include "Visualizer.hpp"
#include <vector>
#include <deque>

class Scheduler {
public:
    std::vector<Process> processes;
    std::deque<Process*> ready_queue;
    Resource resource;
    ISchedulingAlgorithm* algorithm;
    int current_time;
    Process* cpu_current;
    int cpu_current_burst_remaining;
    int cpu_rr_quantum_counter;
    int rr_quantum;
    bool is_rr;
    bool is_srtn;
    std::vector<int> cpu_gantt;
    int total_cpu_idle_time;
    int total_simulation_time;

    Visualizer* visualizer;
    std::string algo_name;

    Scheduler(std::vector<Process> procs, ISchedulingAlgorithm* algo);
    void runSimulation();
    void setVisualizer(Visualizer* vis);

private:
    size_t next_arrival_idx;
    std::deque<Process*> arrivals_this_tick;
    std::deque<Process*> r_returnees_this_tick;

    void handleNewArrivals();
    void processCPU();
    void processResource();
    void resolveConflict();
    void checkSRTNPreemption();
    void pickNextCPUProcess();
    void recordGantt();
    void notifyVisualizer();
    SystemSnapshot buildSnapshot();
    bool isSimulationFinished();
};

#endif
