#ifndef PARSER_HPP
#define PARSER_HPP

#include "Process.hpp"
#include <string>
#include <vector>

class Parser {
public:
    static std::vector<Process> parseInput(const std::string& filepath);
    static void writeOutput(const std::string& filepath,
                            const std::vector<int>& cpu_gantt,
                            const std::vector<int>& res_gantt);
};

#endif
