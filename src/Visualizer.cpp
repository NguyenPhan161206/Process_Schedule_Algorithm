#include "Visualizer.hpp"
#include <iostream>
#include <iomanip>
#include <queue>
#include <thread>
#include <chrono>
#include <algorithm>

Visualizer::Visualizer(bool en)
    : enabled(en)
    , RED("\033[31m")
    , GREEN("\033[32m")
    , YELLOW("\033[33m")
    , CYAN("\033[36m")
    , RESET("\033[0m")
    , BOLD("\033[1m")
{}

void Visualizer::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

std::string Visualizer::color(const std::string& text, const std::string& ansi_code) const {
    return ansi_code + text + RESET;
}

std::string Visualizer::progressBar(int current, int total, int width) const {
    if (total <= 0) total = 1;
    int filled = current * width / total;
    std::string bar = "[";
    for (int i = 0; i < width; ++i) {
        bar += (i < filled) ? '#' : '.';
    }
    bar += "]";
    return bar;
}

static std::string padTo(int target_width, const std::string& content) {
    int content_len = static_cast<int>(content.length());
    int pad = target_width - content_len;
    return (pad > 0) ? std::string(static_cast<size_t>(pad), ' ') : std::string();
}

void Visualizer::render(const SystemSnapshot& snap) {
    if (!enabled) return;

    clearScreen();
    printHeader(snap.current_time);
    printCPUSection(snap.cpu_process, snap.cpu_remaining, snap.cpu_burst_total,
                    snap.algo_name, snap.quantum);
    printResourceSection(snap.res_process, snap.res_remaining, snap.res_burst_total);
    printReadyQueue(snap.ready_queue);
    printResourceQueue(snap.res_queue);

    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Visualizer::printHeader(int time) const {
    std::cout << color(BOLD + "╔══════════════════ OS SCHEDULER SIMULATOR ══════════════════╗", CYAN) << "\n";

    std::string time_line = "  " + BOLD + "⏱ Time: t = " + std::to_string(time) + RESET;
    std::cout << color("║", CYAN) << time_line
              << padTo(56, time_line)
              << color("║", CYAN) << "\n";

    std::cout << color("╚══════════════════════════════════════════════════════════════╝", CYAN) << "\n";
}

void Visualizer::printCPUSection(Process* cpu, int remaining, int total,
                                 const std::string& algo, int quantum) const {
    std::cout << color("┌─ CPU ─────────────────────────────────────────────────┐", CYAN) << "\n";

    if (cpu != nullptr) {
        int used = total - remaining;
        std::string running_text = "Running: P" + std::to_string(cpu->id);
        std::string bar = progressBar(used, total);
        std::string time_info = std::to_string(used) + "/" + std::to_string(total) + "s";

        std::string line = "  " + color(running_text, GREEN) + "  " + bar + " " + time_info;
        std::cout << "│" << line << padTo(56, line) << "│\n";
    } else {
        std::string line = "  " + color("Idle", RED);
        std::cout << "│" << line << padTo(56, line) << "│\n";
    }

    std::string algo_text = "▸ Algorithm: " + algo;
    if (quantum > 0) {
        algo_text += " (Quantum=" + std::to_string(quantum) + ")";
    }
    std::string line = "  " + algo_text;
    std::cout << "│" << line << padTo(56, line) << "│\n";

    std::cout << color("└────────────────────────────────────────────────────────┘", CYAN) << "\n";
}

void Visualizer::printResourceSection(Process* res, int remaining, int total) const {
    std::cout << color("┌─ Resource R ───────────────────────────────────────────┐", CYAN) << "\n";

    if (res != nullptr) {
        int used = total - remaining;
        std::string running_text = "Running: P" + std::to_string(res->id);
        std::string bar = progressBar(used, total);
        std::string time_info = std::to_string(used) + "/" + std::to_string(total) + "s  (FCFS)";

        std::string line = "  " + color(running_text, GREEN) + "  " + bar + " " + time_info;
        std::cout << "│" << line << padTo(56, line) << "│\n";
    } else {
        std::string line = "  " + color("Idle", RED);
        std::cout << "│" << line << padTo(56, line) << "│\n";
    }

    std::cout << color("└────────────────────────────────────────────────────────┘", CYAN) << "\n";
}

void Visualizer::printReadyQueue(const std::deque<Process*>& rq) const {
    std::cout << color("┌─ Ready Queue (CPU) ────────────────────────────────────┐", CYAN) << "\n";

    if (rq.empty()) {
        std::string line = "  " + color("(empty)", YELLOW);
        std::cout << "│" << line << padTo(56, line) << "│\n";
    } else {
        std::string q_text;
        for (size_t i = 0; i < rq.size(); ++i) {
            if (i > 0) q_text += " → ";
            q_text += "[P" + std::to_string(rq[i]->id) + "]";
        }

        std::string line = "  " + color(q_text, YELLOW);
        if (q_text.length() > 50) {
            line = "  " + color(q_text.substr(0, 48) + "…", YELLOW);
        }
        std::cout << "│" << line << padTo(56, line) << "│\n";
    }

    std::cout << color("└────────────────────────────────────────────────────────┘", CYAN) << "\n";
}

void Visualizer::printResourceQueue(const std::queue<Process*>& res_q) const {
    std::cout << color("┌─ Resource Queue (R) ───────────────────────────────────┐", CYAN) << "\n";

    if (res_q.empty()) {
        std::string line = "  " + color("(empty)", YELLOW);
        std::cout << "│" << line << padTo(56, line) << "│\n";
    } else {
        std::queue<Process*> temp = res_q;
        std::string q_text;
        bool first = true;
        while (!temp.empty()) {
            if (!first) q_text += " → ";
            q_text += "[P" + std::to_string(temp.front()->id) + "]";
            temp.pop();
            first = false;
        }

        std::string line = "  " + color(q_text, YELLOW);
        if (q_text.length() > 50) {
            line = "  " + color(q_text.substr(0, 48) + "…", YELLOW);
        }
        std::cout << "│" << line << padTo(56, line) << "│\n";
    }

    std::cout << color("└────────────────────────────────────────────────────────┘", CYAN) << "\n";
}
