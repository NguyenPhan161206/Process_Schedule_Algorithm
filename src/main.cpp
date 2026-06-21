#include <iostream>
#include <string>
#include <cstring>
#include "Parser.hpp"
#include "Scheduler.hpp"
#include "Algorithms.hpp"
#include "Visualizer.hpp"
#include "Analytics.hpp"

static const char* DEFAULT_INPUT  = "test_cases/input_basic.txt";
static const char* DEFAULT_OUTPUT = "output/gantt.txt";
static const int   DEFAULT_QUANTUM = 2;

static bool showMenu(int& algo_code, std::string& input_path,
                     std::string& output_path, int& quantum,
                     bool& tui_mode) {
    std::string line;

    std::cout << "\033[36m╔═══════════════════════════════════════╗\033[0m\n";
    std::cout << "\033[36m║\033[0m  \033[1mOS SCHEDULER SIMULATOR\033[0m            \033[36m║\033[0m\n";
    std::cout << "\033[36m║\033[0m  Interactive Mode                   \033[36m║\033[0m\n";
    std::cout << "\033[36m╚═══════════════════════════════════════╝\033[0m\n\n";

    std::cout << "1) FCFS\n2) Round Robin\n3) SJF\n4) SRTN\n";
    while (true) {
        std::cout << "Chon thuat toan [1]: ";
        std::getline(std::cin, line);
        if (line.empty()) { algo_code = 1; break; }
        try {
            algo_code = std::stoi(line);
            if (algo_code >= 1 && algo_code <= 4) break;
        } catch (...) {}
        std::cout << "Nhap so tu 1 den 4.\n";
    }

    std::cout << "File input [" << DEFAULT_INPUT << "]: ";
    std::getline(std::cin, line);
    input_path = line.empty() ? DEFAULT_INPUT : line;

    std::cout << "File output [" << DEFAULT_OUTPUT << "]: ";
    std::getline(std::cin, line);
    output_path = line.empty() ? DEFAULT_OUTPUT : line;

    if (algo_code == 2) {
        std::cout << "Quantum [" << DEFAULT_QUANTUM << "]: ";
        std::getline(std::cin, line);
        quantum = line.empty() ? DEFAULT_QUANTUM : std::stoi(line);
    }

    std::cout << "Bat TUI? (y/N): ";
    std::getline(std::cin, line);
    tui_mode = (line == "y" || line == "Y");

    std::cout << "\n--- Thong so da nhap ---\n";
    std::cout << "Thuat toan: " << algo_code << "\n";
    std::cout << "Input: " << input_path << "\n";
    std::cout << "Output: " << output_path << "\n";
    if (algo_code == 2) std::cout << "Quantum: " << quantum << "\n";
    std::cout << "TUI: " << (tui_mode ? "Bat" : "Tat") << "\n\n";
    return true;
}

int main(int argc, char* argv[]) {
    int algo_code;
    std::string input_path, output_path;
    int quantum = DEFAULT_QUANTUM;
    bool tui_mode = false;

    if (argc == 1) {
        if (!showMenu(algo_code, input_path, output_path, quantum, tui_mode)) {
            return 1;
        }
        std::cout << "Dang xu ly...\n\n";
    } else {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0]
                      << " <input_file> <output_file> <algorithm_code> [quantum] [--tui]"
                      << std::endl;
            std::cerr << "  algorithm_code: 1=FCFS, 2=RR, 3=SJF, 4=SRTN" << std::endl;
            return 1;
        }

        input_path = argv[1];
        output_path = argv[2];
        algo_code = std::stoi(argv[3]);

        if (algo_code < 1 || algo_code > 4) {
            std::cerr << "Error: algorithm_code must be 1(FCFS), 2(RR), 3(SJF), or 4(SRTN)"
                      << std::endl;
            return 1;
        }

        for (int i = 4; i < argc; ++i) {
            if (std::strcmp(argv[i], "--tui") == 0) {
                tui_mode = true;
            } else {
                quantum = std::stoi(argv[i]);
            }
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
