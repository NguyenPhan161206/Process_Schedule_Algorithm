#include "Parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Process> Parser::parseInput(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) break;
    }
    if (line.empty()) {
        throw std::runtime_error("Input file is empty");
    }

    int N = std::stoi(line);
    std::vector<Process> processes;
    processes.reserve(N);

    int pid = 1;
    while (pid <= N && std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::vector<int> tokens;
        int val;
        while (iss >> val) {
            tokens.push_back(val);
        }

        if (tokens.empty()) continue;

        int arrival = tokens[0];
        Process proc(pid, arrival);

        for (size_t i = 1; i < tokens.size(); i += 2) {
            int cpu_burst = tokens[i];
            proc.bursts.emplace_back(BurstType::CPU, cpu_burst);

            if (i + 1 < tokens.size()) {
                int res_burst = tokens[i + 1];
                proc.bursts.emplace_back(BurstType::RESOURCE, res_burst);
            }
        }

        processes.push_back(proc);
        ++pid;
    }

    return processes;
}

void Parser::writeOutput(const std::string& filepath,
                         const std::vector<int>& cpu_gantt,
                         const std::vector<int>& res_gantt) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open output file: " + filepath);
    }

    for (size_t i = 0; i < cpu_gantt.size(); ++i) {
        if (i > 0) file << ' ';
        if (cpu_gantt[i] == -1) {
            file << '_';
        } else {
            file << cpu_gantt[i];
        }
    }
    file << '\n';

    for (size_t i = 0; i < res_gantt.size(); ++i) {
        if (i > 0) file << ' ';
        if (res_gantt[i] == -1) {
            file << '_';
        } else {
            file << res_gantt[i];
        }
    }
    file << '\n';
}
