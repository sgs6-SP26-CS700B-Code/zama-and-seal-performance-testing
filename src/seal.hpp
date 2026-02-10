#pragma once

#include <cstdint>
#include <vector>

// Will implement this first
void seal_bgv_test_driver(const std::vector<int64_t>& data);

// Will implement after BGV
void seal_ckks_test_driver(const std::vector<int64_t>& data);

// Will implement after ckks, note this will only be available in V4, i dont think any version of V3 supports it, this was the main V4 feature.
void seal_BFV_test_driver(const std::vector<int64_t>& data);


// Will see when I get to this
void seal_batch_bgv_test_driver(const std::vector<int64_t>& data);