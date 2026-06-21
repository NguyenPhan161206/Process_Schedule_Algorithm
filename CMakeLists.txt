cmake_minimum_required(VERSION 3.10)
project(OS_Scheduler)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(scheduler
    src/main.cpp
    src/Process.cpp
    src/Parser.cpp
    src/Resource.cpp
    src/ISchedulingAlgorithm.cpp
    src/Algorithms.cpp
    src/Scheduler.cpp
    src/Visualizer.cpp
    src/Analytics.cpp
)

target_include_directories(scheduler PRIVATE include)
