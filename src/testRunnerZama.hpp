#pragma once

#include <cstdint>
#include <string>
#include <vector>

void testZama_32_no_mult(size_t dataQuantity, const std::string& testName, const std::vector<int32_t>& data);

void testZama_64_no_mult(size_t dataQuantity, const std::string& testName, const std::vector<int64_t>& data);

void testZama_32_in_64_mult(size_t dataQuantity, const std::string& testName, const std::vector<int64_t>& data);

void bulk_zama_tests(
    const std::vector<int64_t>& data64, const std::vector<int32_t>& data32, const std::vector<int64_t>& data32in64);