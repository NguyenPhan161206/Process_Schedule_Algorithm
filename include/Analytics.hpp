#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include "Process.hpp"
#include <vector>
#include <string>

struct ProcessMetrics {
    int pid;
    double response_time;
    double waiting_time;
    double turnaround_time;
};

struct SystemMetrics {
    double cpu_utilization;
    double resource_utilization;
    double avg_response_time;
    double avg_waiting_time;
    double avg_turnaround_time;
};

class Analytics {
public:
    std::vector<Process> processes;
    std::vector<int> cpu_gantt;
    std::vector<int> res_gantt;
    int total_simulation_time;
    int total_cpu_idle_time;
    int total_res_idle_time;
    std::string algo_name;
    int quantum;

    Analytics(const std::vector<Process>& procs,
              int sim_time,
              int cpu_idle,
              int res_idle,
              const std::string& algo,
              int q,
              const std::vector<int>& cpu_g,
              const std::vector<int>& res_g);

    std::vector<ProcessMetrics> calculateProcessMetrics();
    SystemMetrics calculateSystemMetrics(const std::vector<ProcessMetrics>& pm);
    void printReport();
    void printGanttChart();
};

#endif
