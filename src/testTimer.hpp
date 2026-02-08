
#pragma once

#include <string>
#include <chrono>

// Method to print the timing results in multiple units
void printTimingResults(const std::chrono::high_resolution_clock::time_point& start,
                        const std::chrono::high_resolution_clock::time_point& end,
                        const std::string& testName);