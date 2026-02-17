
#include <testTimer.hpp>
#include <seal.hpp>
#include <fileProcessingHelpers.hpp>
#include <testRunnerSeal.hpp>


#include <chrono>
#include <vector>
#include <iostream>
#include <string>

using namespace std;


void bulk_seal_tests(
    const std::vector<int64_t>& data64, const std::vector<int32_t>& data32, const std::vector<int64_t>& data32in64)
{

    cout << "Begining SEAL Tests: Note, this runs on 1 thread, and current parameter configuration does not always "
            "have accurate results... Need to figure out fine tuning better, but I currently suspect this is from "
            "overflows or underflows"
         << endl;



    testSeal_32_batch(1024, "SEAL 512 Batch 32 bit", data32);
    testSeal_32_batch(2048, "SEAL 1024 Batch 32 bit", data32);
    testSeal_32_batch(4096, "SEAL 2048 Batch 32 bit", data32);
    testSeal_32_batch(8192, "SEAL 4096 Batch 32 bit", data32);
    testSeal_32_batch(16384, "SEAL 8192 Batch 32 bit", data32);
    testSeal_32_batch(32768, "SEAL 16384 Batch 32 bit", data32);
    testSeal_32_batch(32768 * 2, "SEAL 32768 Batch 32 bit", data32);

    testSeal_32(1000, "Seal 1k 32 bit", data32);
    testSeal_32(10000, "Seal 10k 32 bit", data32);
    testSeal_32(100000, "Seal 100k 32 bit", data32);
    testSeal_32(250000, "Seal 250k 32 bit", data32);
    testSeal_32(500000, "Seal 500k 32 bit", data32);
    testSeal_32(1000000, "Seal 1m 32 bit", data32);

    // cout << " Skipping the following normal tests for time limitations: 10k, 100k, 250k, 500k, 1m";
}

void testSeal_32(size_t dataQuantity, const string& testName, const std::vector<int32_t>& data)
{
    auto dataToTest = getFirstNValues32(data, dataQuantity);
    cout << endl
         << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;

    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    cout << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=[SEAL BFV 32]=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    seal_bfv_test_driver_32_no_mult(dataToTest);
    //seal_bfv_test_driver_32_fixed_mults(dataToTest); // No clue why, but this is never accurate at the minute.
    auto end = std::chrono::high_resolution_clock::now(); // Record end time
    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName + " BFV");

    start = std::chrono::high_resolution_clock::now(); // Record start time
    cout << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=[SEAL BGV 32]=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    seal_bgv_test_driver_32_no_mult(dataToTest);
    //seal_bgv_test_driver_32_fixed_mults(dataToTest); // No clue why, but this is never accurate at the minute.
    end = std::chrono::high_resolution_clock::now(); // Record end time
    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName + " BGV");


    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}


void testSeal_32_batch(size_t dataQuantity, const string& testName, const std::vector<int32_t>& data)
{
    auto dataToTest = getFirstNValues32(data, dataQuantity);
    cout << endl
         << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;

    auto start = std::chrono::high_resolution_clock::now(); // Record start time
    cout << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=[SEAL BFV 32 batch]=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    seal_bfv_test_driver_32_no_mult_batch(dataToTest);
    auto end = std::chrono::high_resolution_clock::now(); // Record end time
    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName + " BFV");


    start = std::chrono::high_resolution_clock::now(); // Record start time
    cout << endl
         << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=[SEAL BGV 32 batch]=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    seal_bgv_test_driver_32_no_mult_batch(dataToTest);
    end = std::chrono::high_resolution_clock::now(); // Record end time
    // Pass the start time, end time, and test name to the print function
    printTimingResults(start, end, testName + " BGV");



    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl;
    cout << "|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|" << endl << endl;
}