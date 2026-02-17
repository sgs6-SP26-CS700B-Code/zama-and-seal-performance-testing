

#include <testTimer.hpp>
#include <zama.hpp>
#include <fileProcessingHelpers.hpp>
#include <testRunnerZama.hpp>


#include <chrono>
#include <vector>
#include <iostream>
#include <string>

using namespace std;


void testZama_32_no_mult(size_t dataQuantity, const string& testName, const std::vector<int32_t>& data)
{
    auto dataToTest = getFirstNValues32(data, dataQuantity);
    cout << endl
         << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    zama_test_driver_no_mult_32(dataToTest);
    auto end = std::chrono::high_resolution_clock::now(); // Record end time

    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName);

    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}


void testZama_64_no_mult(size_t dataQuantity, const string& testName, const std::vector<int64_t>& data)
{
    auto dataToTest = getFirstNValues64(data, dataQuantity);
    cout << endl
         << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    zama_test_driver_no_mult(dataToTest);
    auto end = std::chrono::high_resolution_clock::now(); // Record end time

    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName);

    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}


void testZama_32_in_64_mult(size_t dataQuantity, const string& testName, const std::vector<int64_t>& data)
{
    auto dataToTest = getFirstNValues64(data, dataQuantity);
    cout << endl
         << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    // Sample function you want to time
    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    zama_test_driver_mult(dataToTest);
    auto end = std::chrono::high_resolution_clock::now(); // Record end time

    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName);

    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}


void bulk_zama_tests(
    const std::vector<int64_t>& data64, const std::vector<int32_t>& data32, const std::vector<int64_t>& data32in64)
{

    cout << "Begining Zama Tests: Note, this runs on 16 threads, will take more work to limit" << endl;

    testZama_32_no_mult(1000, "Zama 1k 32 bit no mult", data32);
    testZama_64_no_mult(1000, "Zama 1k 32 bit in 64 buffer no mult", data64);
    testZama_32_in_64_mult(1000, "Zama 1k 32 bit in 64 buffer only mult", data32in64);


    testZama_32_no_mult(10000, "Zama 10k 32 bit no mult", data32);
    testZama_64_no_mult(10000, "Zama 10k 32 bit in 64 buffer no mult", data64);


    cout << " Skipping the following tests for time limitations: 10k mult, 100k, 250k, 500k, 1m";
}