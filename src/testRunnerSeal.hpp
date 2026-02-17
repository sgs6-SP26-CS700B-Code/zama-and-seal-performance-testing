#pragma once

#include <cstdint>
#include <string>
#include <vector>


void bulk_seal_tests(
    const std::vector<int64_t>& data64, const std::vector<int32_t>& data32, const std::vector<int64_t>& data32in64);

void testSeal_32(size_t dataQuantity, const std::string& testName, const std::vector<int32_t>& data);
void testSeal_32_batch(size_t dataQuantity, const std::string& testName, const std::vector<int32_t>& data);