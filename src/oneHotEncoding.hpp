
#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace oneHotEncodeing {

static const std::map<uint8_t, uint32_t> digitToOneHotUInt32{
    {0, 0000000001},
    {1, 0000000010},
    {2, 0000000100},
    {3, 0000001000},
    {4, 0000010000},
    {5, 0000100000},
    {6, 0001000000},
    {7, 0010000000},
    {8, 0100000000},
    {9, 1000000000}
};

static const std::map<uint32_t, uint8_t> uInt32OneHotToDigit{
    {0000000001, 0},
    {0000000010, 1},
    {0000000100, 2},
    {0000001000, 3},
    {0000010000, 4},
    {0000100000, 5},
    {0001000000, 6},
    {0010000000, 7},
    {0100000000, 8},
    {1000000000, 9}
};


///-----------------------------------------------------------------------------
/// @brief A struct to define a uInt that takes up 10 bits, assumes big endian.
/// Note, of course, struct itself takes up 16 bits, but we dont care
/// as this is just to store oneHotEncoded values and the actual work that
/// will be computed on will be 10 bit.
///
/// @note Although we can use the bitset feature, this is more portable to C
/// and will be easier for the API / rapid prototyping.
///-----------------------------------------------------------------------------
struct tenBitUInt {
    uint16_t tenBitUInt : 10;
};

static const std::map<uint8_t, tenBitUInt> digitToOneHotUInt10{
    {0, {0b0000000001}},
    {1, {0b0000000010}},
    {2, {0b0000000100}},
    {3, {0b0000001000}},
    {4, {0b0000010000}},
    {5, {0b0000100000}},
    {6, {0b0001000000}},
    {7, {0b0010000000}},
    {8, {0b0100000000}},
    {9, {0b1000000000}}
};

struct CustomCompare {
    bool operator()(const tenBitUInt& a, const tenBitUInt& b) const
    {
        // Custom logic: sort by string length
        return a.tenBitUInt < b.tenBitUInt;
    }
};


static const std::map<tenBitUInt, uint8_t, CustomCompare> uInt10OneHotToDigit{
    {{0b0000000001}, 0},
    {{0b0000000010}, 1},
    {{0b0000000100}, 2},
    {{0b0000001000}, 3},
    {{0b0000010000}, 4},
    {{0b0000100000}, 5},
    {{0b0001000000}, 6},
    {{0b0010000000}, 7},
    {{0b0100000000}, 8},
    {{0b1000000000}, 9}
};

using oneHotUInt32Vec = std::vector<uint32_t>;
using oneHotUInt10Vec = std::vector<tenBitUInt>;

///-----------------------------------------------------------------------------
/// @brief Extracts each digit in a uint32_t in big endian format
///
/// @param[in] num = the uint32_t number
/// @return std::vector<uint8_t> = the broken up digits in big endian
///-----------------------------------------------------------------------------
std::vector<uint8_t> extractEachDigit(uint32_t num);

///-----------------------------------------------------------------------------
/// @brief Extracts each digit in a uint32_t in big endian format
///
/// @param[in] num = the uint32_t number
/// @return std::vector<uint8_t> = the broken up digits in big endian
///-----------------------------------------------------------------------------
std::array<uint8_t, 10> extractEachDigitArr(uint32_t num);

///-----------------------------------------------------------------------------
/// @brief Inverse of extractEachDigit, creates a number from digits
///
/// @param[in] digits = the vector of digits in big endian
/// @return uint32_t = the completed number
///-----------------------------------------------------------------------------
uint32_t digitVectorToUInt32_t(const std::vector<uint8_t>& digits);

oneHotUInt32Vec oneHotEncodeUInt32(uint32_t num);
oneHotUInt10Vec oneHotEncodeUInt10(uint32_t num);
uint32_t        oneHotDecodeUInt32(const oneHotUInt32Vec& num);
uint32_t        oneHotDecodeUInt10(const oneHotUInt10Vec& num);


uint32_t oneHotAddWithoutNormalize(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b);

}; // namespace oneHotEncodeing