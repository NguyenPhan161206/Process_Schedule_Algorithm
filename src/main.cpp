#include <iostream>
#include <string>
#include <cstring>
#include "Parser.hpp"
#include "Scheduler.hpp"
#include "Algorithms.hpp"
#include "Visualizer.hpp"
#include "Analytics.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <input_file> <output_file> <algorithm_code> [quantum] [--tui]"
                  << std::endl;
        std::cerr << "  algorithm_code: 1=FCFS, 2=RR, 3=SJF, 4=SRTN" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    int algo_code = std::stoi(argv[3]);

    if (algo_code < 1 || algo_code > 4) {
        std::cerr << "Error: algorithm_code must be 1(FCFS), 2(RR), 3(SJF), or 4(SRTN)"
                  << std::endl;
        return 1;
    }

    int quantum = 2;
    bool tui_mode = false;

    for (int i = 4; i < argc; ++i) {
        if (std::strcmp(argv[i], "--tui") == 0) {
            tui_mode = true;
        } else {
            quantum = std::stoi(argv[i]);
        }
    }

    std::vector<Process> processes;
    try {
        processes = Parser::parseInput(input_path);
    } catch (const std::exception& e) {
        std::cerr << "Error reading input: " << e.what() << std::endl;
        return 1;
    }

    if (processes.empty()) {
        std::cerr << "Error: no processes found in input file" << std::endl;
        return 1;
    }

    ISchedulingAlgorithm* algo = nullptr;
    std::string algo_name;
    switch (algo_code) {
        case 1:
            algo = new FCFSScheduler();
            algo_name = "FCFS";
            break;
        case 2:
            algo = new RoundRobinScheduler(quantum);
            algo_name = "Round Robin";
            break;
        case 3:
            algo = new SJFScheduler();
            algo_name = "SJF";
            break;
        case 4:
            algo = new SRTNScheduler();
            algo_name = "SRTN";
            break;
    }

    Visualizer visualizer(tui_mode);

    Scheduler scheduler(processes, algo);
    scheduler.algo_name = algo_name;
    scheduler.setVisualizer(&visualizer);

    scheduler.runSimulation();

    try {
        Parser::writeOutput(output_path, scheduler.cpu_gantt, scheduler.resource.gantt_history);
    } catch (const std::exception& e) {
        std::cerr << "Error writing output: " << e.what() << std::endl;
        delete algo;
        return 1;
    }

    int analytics_quantum = (algo_code == 2) ? quantum : 0;
    Analytics analytics(scheduler.processes,
                        scheduler.total_simulation_time,
                        scheduler.total_cpu_idle_time,
                        scheduler.resource.total_idle_time,
                        algo_name, analytics_quantum);
    analytics.printReport();

    delete algo;
    return 0;
}
