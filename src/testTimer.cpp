
#include <testTimer.hpp>
#include <iostream>
#include <string>
#include <chrono>

// Method to print the timing results in multiple units
void printTimingResults(
    const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end,
    const std::string&                                    testName)
{
    auto duration = end - start;

    auto seconds      = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    auto nanoseconds  = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    std::cout << "\n";
    std::cout << "==========================================================================\n";
    std::cout << "  Timing Results for: " << testName << "\n";
    std::cout << "==========================================================================\n";

    std::cout << "\t| Time taken (seconds)      | " << seconds.count() << "s\n";
    std::cout << "\t| Time taken (milliseconds) | " << milliseconds.count() << "ms\n";
    std::cout << "\t| Time taken (microseconds) | " << microseconds.count() << "us\n";
    std::cout << "\t| Time taken (nanoseconds)  | " << nanoseconds.count() << "ns\n";

    std::cout << "--------------------------------------------------------------------------\n";
}



void printSizeInUnits(size_t sizeInBytes, const std::string& testName)
{
    std::cout << "\n";
    std::cout << "==========================================================================\n";
    std::cout << "  Size Information for: " << testName << "\n";
    std::cout << "==========================================================================\n";

    // Convert to various units
    size_t sizeInBits = sizeInBytes * 8;
    size_t sizeInKB   = sizeInBytes / 1024;
    size_t sizeInMB   = sizeInKB / 1024;

    std::cout << "\t| Size (bits)        | " << sizeInBits << " bits\n";
    std::cout << "\t| Size (bytes)       | " << sizeInBytes << " bytes\n";
    std::cout << "\t| Size (kilobytes)   | " << sizeInKB << " KB\n";
    std::cout << "\t| Size (megabytes)   | " << sizeInMB << " MB\n";

    std::cout << "--------------------------------------------------------------------------\n";
}