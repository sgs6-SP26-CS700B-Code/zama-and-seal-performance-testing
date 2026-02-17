#pragma once

#include <cstdint>
#include <vector>

void seal_bfv_test_driver_32_no_mult(const std::vector<int32_t>& data);
void seal_bfv_test_driver_32_fixed_mults(const std::vector<int32_t>& data); // Not yet used, params are very bad
void seal_bfv_test_driver_32_no_mult_batch(const std::vector<int32_t>& data);

// Will implement after BGV
// void seal_ckks_test_driver(const std::vector<int64_t>& data);

void seal_bgv_test_driver_32_no_mult(const std::vector<int32_t>& data);
void seal_bgv_test_driver_32_fixed_mults(const std::vector<int32_t>& data); // Not yet used, params are very bad
void seal_bgv_test_driver_32_no_mult_batch(const std::vector<int32_t>& data);


// // Will see when I get to this
// void seal_batch_bgv_test_driver(const std::vector<int64_t>& data);