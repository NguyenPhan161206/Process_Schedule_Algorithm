#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "Process.hpp"
#include "Resource.hpp"
#include <deque>
#include <queue>
#include <vector>
#include <string>

struct SystemSnapshot {
    int current_time;
    Process* cpu_process;
    int cpu_remaining;
    int cpu_burst_total;
    Process* res_process;
    int res_remaining;
    int res_burst_total;
    std::deque<Process*> ready_queue;
    std::queue<Process*> res_queue;
    std::string algo_name;
    int quantum;
};

class Visualizer {
public:
    bool enabled;

    explicit Visualizer(bool en = false);
    void render(const SystemSnapshot& snapshot);
    void clearScreen();

private:
    std::string progressBar(int current, int total, int width = 10) const;
    std::string color(const std::string& text, const std::string& ansi_code) const;

    void printHeader(int time) const;
    void printCPUSection(Process* cpu, int remaining, int total,
                         const std::string& algo, int quantum) const;
    void printResourceSection(Process* res, int remaining, int total) const;
    void printReadyQueue(const std::deque<Process*>& rq) const;
    void printResourceQueue(const std::queue<Process*>& res_q) const;

    std::string RED;
    std::string GREEN;
    std::string YELLOW;
    std::string CYAN;
    std::string RESET;
    std::string BOLD;
};

#endif
