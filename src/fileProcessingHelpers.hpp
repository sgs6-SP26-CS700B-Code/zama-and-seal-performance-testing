#pragma once

#include <cstdint>
#include <string>
#include <vector>

//==============================[int32 & int64 data reading & spliting]=======================================

std::vector<int32_t> getFirstNValues32(const std::vector<int32_t>& input, size_t N);

std::vector<int64_t> getFirstNValues64(const std::vector<int64_t>& input, size_t N);

std::vector<int64_t> readFileToInt64Vector(const std::string& filename, size_t maxLines);

std::vector<int32_t> readFileToInt32Vector(const std::string& filename, size_t maxLines);

//==============================[uint32 & uint64 data reading & spliting]=======================================


std::vector<uint32_t> getFirstNValuesU32(const std::vector<uint32_t>& input, size_t N);

std::vector<uint64_t> getFirstNValuesU64(const std::vector<uint64_t>& input, size_t N);

std::vector<uint64_t> readFileToUInt64Vector(const std::string& filename, size_t maxLines);

std::vector<uint32_t> readFileToUInt32Vector(const std::string& filename, size_t maxLines);