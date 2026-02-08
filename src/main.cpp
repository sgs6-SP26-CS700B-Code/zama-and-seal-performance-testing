
#include <testTimer.cpp>
#include <zama.hpp>
#include <chrono>
#include <string>


int main() {
    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now();  // Record start time
    zama_test_driver();
    auto end = std::chrono::high_resolution_clock::now();  // Record end time

    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, "Sample Test");


    return 0;
}