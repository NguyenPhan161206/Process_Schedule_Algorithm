#include "Analytics.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

Analytics::Analytics(const std::vector<Process>& procs,
                     int sim_time,
                     int cpu_idle,
                     int res_idle,
                     const std::string& algo,
                     int q)
    : processes(procs)
    , total_simulation_time(sim_time)
    , total_cpu_idle_time(cpu_idle)
    , total_res_idle_time(res_idle)
    , algo_name(algo)
    , quantum(q)
{}

std::vector<ProcessMetrics> Analytics::calculateProcessMetrics() {
    std::vector<ProcessMetrics> metrics;
    metrics.reserve(processes.size());

    for (const auto& p : processes) {
        ProcessMetrics pm;
        pm.pid = p.id;
        pm.response_time = static_cast<double>(p.first_response_time - p.arrival_time);
        pm.waiting_time = static_cast<double>(p.total_waiting_time);
        pm.turnaround_time = static_cast<double>(p.completion_time - p.arrival_time);
        metrics.push_back(pm);
    }

    return metrics;
}

SystemMetrics Analytics::calculateSystemMetrics(const std::vector<ProcessMetrics>& pm) {
    SystemMetrics sm;

    if (total_simulation_time > 0) {
        sm.cpu_utilization = (1.0 - static_cast<double>(total_cpu_idle_time) / total_simulation_time) * 100.0;
        sm.resource_utilization = (1.0 - static_cast<double>(total_res_idle_time) / total_simulation_time) * 100.0;
    } else {
        sm.cpu_utilization = 0.0;
        sm.resource_utilization = 0.0;
    }

    double sum_r = 0.0, sum_w = 0.0, sum_t = 0.0;
    for (const auto& m : pm) {
        sum_r += m.response_time;
        sum_w += m.waiting_time;
        sum_t += m.turnaround_time;
    }

    int n = static_cast<int>(pm.size());
    if (n > 0) {
        sm.avg_response_time = sum_r / n;
        sm.avg_waiting_time = sum_w / n;
        sm.avg_turnaround_time = sum_t / n;
    } else {
        sm.avg_response_time = 0.0;
        sm.avg_waiting_time = 0.0;
        sm.avg_turnaround_time = 0.0;
    }

    return sm;
}

void Analytics::printReport() {
    std::vector<ProcessMetrics> pm = calculateProcessMetrics();
    SystemMetrics sm = calculateSystemMetrics(pm);

    const std::string H = "\033[36m";
    const std::string B = "\033[1m";
    const std::string R = "\033[0m";

    std::cout << std::fixed << std::setprecision(2);

    // Header
    std::cout << H << "╔══════════════════════ PERFORMANCE ANALYTICS ══════════════════╗" << R << "\n";
    std::cout << H << "║" << R << "  " << B << "Algorithm: " << algo_name;
    if (quantum > 0) {
        std::cout << " (Quantum=" << quantum << ")";
    }
    std::cout << R << std::string(55 - algo_name.length() - (quantum > 0 ? 13 : 0), ' ')
              << H << "║" << R << "\n";
    std::cout << H << "║" << R << "  " << "Total Simulation Time: " << total_simulation_time << "s"
              << std::string(40 - std::to_string(total_simulation_time).length(), ' ')
              << H << "║" << R << "\n";
    std::cout << H << "║" << R << "  " << std::string(56, ' ') << H << "║" << R << "\n";

    // Table header
    std::cout << H << "║" << R
              << "  ┌─────┬──────────┬──────────┬──────────────┐"
              << std::string(7, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  │ PID │ Response │ Waiting  │ Turnaround   │"
              << std::string(7, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  ├─────┼──────────┼──────────┼──────────────┤"
              << std::string(7, ' ') << H << "║" << R << "\n";

    // Process rows
    for (const auto& m : pm) {
        std::cout << H << "║" << R
                  << "  │ "
                  << "P" << std::left << std::setw(2) << m.pid << std::right
                  << " │ "
                  << std::setw(8) << m.response_time
                  << " │ "
                  << std::setw(8) << m.waiting_time
                  << " │ "
                  << std::setw(12) << m.turnaround_time
                  << " │"
                  << std::string(7, ' ') << H << "║" << R << "\n";
    }

    // Table footer
    std::cout << H << "║" << R
              << "  ├─────┼──────────┼──────────┼──────────────┤"
              << std::string(7, ' ') << H << "║" << R << "\n";

    // Average row
    std::cout << H << "║" << R
              << "  │ "
              << "Avg"
              << " │ "
              << std::setw(8) << sm.avg_response_time
              << " │ "
              << std::setw(8) << sm.avg_waiting_time
              << " │ "
              << std::setw(12) << sm.avg_turnaround_time
              << " │"
              << std::string(7, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  └─────┴──────────┴──────────┴──────────────┘"
              << std::string(7, ' ') << H << "║" << R << "\n";

    // Utilization
    std::cout << H << "║" << R << "  " << std::string(56, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  ┌──────────────────────────────────────┐"
              << std::string(14, ' ') << H << "║" << R << "\n";

    std::string cpu_str = "CPU Utilization:    " + std::to_string(sm.cpu_utilization).substr(0, 5) + "%";
    std::string res_str = "Resource Utilization: " + std::to_string(sm.resource_utilization).substr(0, 5) + "%";
    std::cout << H << "║" << R
              << "  │ " << cpu_str
              << std::string(38 - cpu_str.length(), ' ') << "│"
              << std::string(14, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  │ " << res_str
              << std::string(38 - res_str.length(), ' ') << "│"
              << std::string(14, ' ') << H << "║" << R << "\n";
    std::cout << H << "║" << R
              << "  └──────────────────────────────────────┘"
              << std::string(14, ' ') << H << "║" << R << "\n";

    std::cout << H << "╚══════════════════════════════════════════════════════════════╝" << R << "\n";
}
