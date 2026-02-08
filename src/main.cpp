
#include <testTimer.hpp>
#include <zama.hpp>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

std::vector<int64_t> readFileToInt64Vector(const std::string& filename) {
    std::vector<int64_t> result;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    int64_t tempValue;  // Temporary variable to hold signed values
    while (file >> tempValue) {
        result.push_back(tempValue);
    }

    file.close();
    return result;
}

int main() {
    const string dataFile= "./1m-64-bit-data.data";
    auto signed64BitData = readFileToInt64Vector(dataFile);

    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now();  // Record start time
    zama_test_driver(signed64BitData);
    auto end = std::chrono::high_resolution_clock::now();  // Record end time

    // Pass the start time, end time, and test name to the print function
    timing::printTimingResults(start, end, "Sample Test");


    return 0;
}