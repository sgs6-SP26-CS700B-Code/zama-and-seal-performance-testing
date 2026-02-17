
#include <fileProcessingHelpers.hpp>


#include <iostream>
#include <fstream>
#include <string>

std::vector<int32_t> getFirstNValues32(const std::vector<int32_t>& input, size_t N)
{
    std::vector<int32_t> result;

    // Ensure we don't exceed the size of the input vector
    size_t count = std::min(N, input.size());

    // Copy the first N elements to the result vector
    result.insert(result.end(), input.begin(), input.begin() + count);

    return result;
}

std::vector<int64_t> getFirstNValues64(const std::vector<int64_t>& input, size_t N)
{
    std::vector<int64_t> result;

    // Ensure we don't exceed the size of the input vector
    size_t count = std::min(N, input.size());

    // Copy the first N elements to the result vector
    result.insert(result.end(), input.begin(), input.begin() + count);

    return result;
}


std::vector<uint32_t> getFirstNValuesU32(const std::vector<uint32_t>& input, size_t N)
{
    std::vector<uint32_t> result;

    // Ensure we don't exceed the size of the input vector
    size_t count = std::min(N, input.size());

    // Copy the first N elements to the result vector
    result.insert(result.end(), input.begin(), input.begin() + count);

    return result;
}

std::vector<uint64_t> getFirstNValuesU64(const std::vector<uint64_t>& input, size_t N)
{
    std::vector<uint64_t> result;

    // Ensure we don't exceed the size of the input vector
    size_t count = std::min(N, input.size());

    // Copy the first N elements to the result vector
    result.insert(result.end(), input.begin(), input.begin() + count);

    return result;
}

std::vector<int64_t> readFileToInt64Vector(const std::string& filename, size_t maxLines)
{
    std::vector<int64_t> result;
    std::ifstream        file(filename);

    if(!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    int64_t tempValue;     // Temporary variable to hold signed values
    size_t  lineCount = 0; // Track the number of lines read

    while(file >> tempValue) {
        result.push_back(tempValue);
        lineCount++;

        // Stop reading once we've reached the max number of lines
        if(lineCount >= maxLines) {
            break;
        }
    }

    file.close();
    return result;
}


std::vector<int32_t> readFileToInt32Vector(const std::string& filename, size_t maxLines)
{
    std::vector<int32_t> result;
    std::ifstream        file(filename);

    if(!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    int32_t tempValue;     // Temporary variable to hold signed values
    size_t  lineCount = 0; // Track the number of lines read

    while(file >> tempValue) {
        result.push_back(tempValue);
        lineCount++;

        // Stop reading once we've reached the max number of lines
        if(lineCount >= maxLines) {
            break;
        }
    }

    file.close();
    return result;
}


std::vector<uint64_t> readFileToUInt64Vector(const std::string& filename, size_t maxLines)
{
    std::vector<uint64_t> result;
    std::ifstream         file(filename);

    if(!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    uint64_t tempValue;     // Temporary variable to hold signed values
    size_t   lineCount = 0; // Track the number of lines read

    while(file >> tempValue) {
        result.push_back(tempValue);
        lineCount++;

        // Stop reading once we've reached the max number of lines
        if(lineCount >= maxLines) {
            break;
        }
    }

    file.close();
    return result;
}


std::vector<uint32_t> readFileToUInt32Vector(const std::string& filename, size_t maxLines)
{
    std::vector<uint32_t> result;
    std::ifstream         file(filename);

    if(!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    uint32_t tempValue;     // Temporary variable to hold signed values
    size_t   lineCount = 0; // Track the number of lines read

    while(file >> tempValue) {
        result.push_back(tempValue);
        lineCount++;

        // Stop reading once we've reached the max number of lines
        if(lineCount >= maxLines) {
            break;
        }
    }

    file.close();
    return result;
}
