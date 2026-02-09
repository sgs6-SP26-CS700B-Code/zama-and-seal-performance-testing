
#include <testTimer.hpp>
#include <zama.hpp>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


std::vector<int64_t> getFirstNValues(const std::vector<int64_t>& input, size_t N) {
    std::vector<int64_t> result;
    
    // Ensure we don't exceed the size of the input vector
    size_t count = std::min(N, input.size());
    
    // Copy the first N elements to the result vector
    result.insert(result.end(), input.begin(), input.begin() + count);
    
    return result;
}

std::vector<int64_t> readFileToInt64Vector(const std::string& filename, size_t maxLines) {
    std::vector<int64_t> result;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    int64_t tempValue;  // Temporary variable to hold signed values
    size_t lineCount = 0;  // Track the number of lines read

    while (file >> tempValue) {
        result.push_back(tempValue);
        lineCount++;

        // Stop reading once we've reached the max number of lines
        if (lineCount >= maxLines) {
            break;
        }
    }

    file.close();
    return result;
}

void testZama(size_t dataQuantity, const string& testName, const std::vector<int64_t>& data){
    auto dataToTest = getFirstNValues(data, dataQuantity);
    cout << endl << endl << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now();  // Record start time
    zama_test_driver(dataToTest);
    auto end = std::chrono::high_resolution_clock::now();  // Record end time

    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName);

    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}

int main() {
    const string dataFile= "./1m-32-bit-data.data";
    cout << "Begining to read 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile << endl;
    auto signed64BitData = readFileToInt64Vector(dataFile, 1000000);

    cout << "Finished reading 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile << endl;

    cout << "Begining Zama Tests: Note, this runs on 16 threads, need more time to modify its automatic threading using its API" << endl;

    testZama(1000, "Zama 1k", signed64BitData);

    cout << " Skipping the following tests for time limitations: 10k, 100k, 250k, 500k, 1m";
    // testZama(10000, "Zama 10k", signed64BitData);
    // testZama(100000, "Zama 100k", signed64BitData);
    // testZama(250000, "Zama 250k", signed64BitData);
    // testZama(500000, "Zama 500k", signed64BitData);
    // testZama(1000000, "Zama 1m", signed64BitData);

    return 0;
}