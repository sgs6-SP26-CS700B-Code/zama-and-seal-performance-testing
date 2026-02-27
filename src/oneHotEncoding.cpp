
#include <cmath>
#include <cstdint>
#include <oneHotEncoding.hpp>
#include <string>
#include <algorithm>
#include <iostream>
#include <array>

namespace oneHotEncodeing {


std::vector<uint8_t> extractEachDigit(uint32_t num)
{
    std::vector<uint8_t> result;

    // Convert the number to string to easily access each digit
    std::string num_str = std::to_string(num);

    // Insert each uint8_tacter of the string into the result vector
    for(uint8_t digit : num_str) {
        result.push_back(static_cast<uint8_t>(digit - '0'));
    }

    // Reverse the vector to make it big-endian (LSB at the end)
    std::reverse(result.begin(), result.end());
    return result;
}


std::array<uint8_t, 10> extractEachDigitArr(uint32_t num)
{
    std::array<uint8_t, 10> result;

    // Convert the number to string to easily access each digit
    std::string num_str = std::to_string(num);

    // Insert each uint8_tacter of the string into the result vector
    int i = 9;
    for(uint8_t digit : num_str) {
        result[i] = digit;
        i--;
    }
    return result;
}

uint32_t digitVectorToUInt32_t(const std::vector<uint8_t>& digits)
{
    uint32_t result = 0;

    // We iterate over the vector of digits (which is big-endian)
    for(size_t i = 0; i < digits.size(); ++i) {
        result += (digits[i] - '0') * static_cast<uint32_t>(pow(10, i));
    }

    return result;
};


oneHotUInt32Vec oneHotEncodeUInt32(uint32_t num)
{
    oneHotUInt32Vec result{};
    auto            digits = extractEachDigit(num);
    for(uint8_t digit : digits) {
        result.push_back(digitToOneHotUInt32.at(digit));
    }
    return result;
};

oneHotUInt10Vec oneHotEncodeUInt10(uint32_t num)
{
    oneHotUInt10Vec result{};
    auto            digits = extractEachDigit(num);
    for(uint8_t digit : digits) {
        result.push_back(digitToOneHotUInt10.at(digit));
    }
    return result;
};

uint32_t oneHotDecodeUInt32(const oneHotUInt32Vec& num)
{
    uint32_t          result{};
    std::vector<uint8_t> digits{};
    for(auto bit32OneHotEncodedDigit : num) {
        digits.push_back(uInt32OneHotToDigit.at(bit32OneHotEncodedDigit));
    }
    result = digitVectorToUInt32_t(digits);
    return result;
};

uint32_t oneHotDecodeUInt10(const oneHotUInt10Vec& num)
{
    uint32_t          result{};
    std::vector<uint8_t> digits{};
    for(auto bit10OneHotEncodedDigit : num) {
        digits.push_back(uInt10OneHotToDigit.at(bit10OneHotEncodedDigit));
    }
    result = digitVectorToUInt32_t(digits);
    return result;
};


// Tensor size (digit range from 0-9)
#define TENSOR_SIZE 10

// Function to add two one-hot encoded uint32_t values (without bitwise operations)
uint32_t oneHotAddWithoutNormalize(uint32_t a, uint32_t b)
{
    uint32_t sum_index = 0;

    // Map the one-hot encoded values back to their corresponding digit indices
    uint8_t digit_a = uInt32OneHotToDigit.at(a);
    uint8_t digit_b = uInt32OneHotToDigit.at(b);

    // Add the indices (digits)
    sum_index = digit_a + digit_b;

    return sum_index;
}

// Function to multiply two one-hot encoded uint32_t values (without bitwise operations)
uint32_t oneHotMultiplyWithoutNormalize(uint32_t a, uint32_t b)
{
    uint32_t product_index = 0;

    // Map the one-hot encoded values back to their corresponding digit indices
    uint8_t digit_a = uInt32OneHotToDigit.at(a);
    uint8_t digit_b = uInt32OneHotToDigit.at(b);

    // Multiply the indices (digits)
    product_index = digit_a * digit_b;

    return product_index;
}


// void printTensor(const std::vector<int>& tensor) {
//     for (int i = 0; i < TENSOR_SIZE; ++i) {
//         std::cout << tensor[i] << " ";
//     }
//     std::cout << std::endl;
// }

} // namespace oneHotEncodeing