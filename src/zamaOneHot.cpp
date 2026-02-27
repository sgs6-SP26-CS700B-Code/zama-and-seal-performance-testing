
// #include <zama.hpp>
#include <testTimer.hpp>
#include <oneHotEncoding.hpp>
#include <zamaOneHot.hpp>

#include "tfhe.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <assert.h>

void zama_test_driver_one_hot_8190_digit(const std::vector<uint32_t>& data)
{

    int ok = 0;
    std::cout << ok << std::endl;

    // Prepare the config builder for the high level API and choose which types to enable
    ConfigBuilder* builder;
    Config*        config;

    // Put the builder in a default state without any types enabled
    config_builder_default(&builder);
    // Populate the config
    config_builder_build(builder, &config);

    ClientKey* client_key = NULL;
    ServerKey* server_key = NULL;

    // Generate the keys using the config
    generate_keys(config, &client_key, &server_key);
    // Set the server key for the current thread
    set_server_key(server_key);

    // Create vectors to hold the encrypted values
    std::vector<FheUint10*> encrypted_data;
    std::vector<FheUint10*> or_result_data;
    std::vector<FheUint10*> and_result_data;

    std::cout << "Begining zama tests" << std::endl;
    std::cout << std::flush;
    //=============================================================================================
    // Begin Tests
    //=============================================================================================

    // Encode and splitup data. Yes this can be more optimized, but this is quick and dirty, and we
    // are less interested in this performance.
    std::vector<oneHotEncodeing::oneHotUInt10Vec> dataEncodedOneHot{};
    for(auto num : data) {
        auto oneNumOneHotEncodedBitVec = oneHotEncodeing::oneHotEncodeUInt10(num);
        dataEncodedOneHot.push_back(oneNumOneHotEncodedBitVec);
    }

    std::vector<oneHotEncodeing::tenBitUInt> oneHotData{};
    for(int i = 0; i < dataEncodedOneHot.size(); ++i) {
        for(auto encodedVal : dataEncodedOneHot[i]) {
            oneHotData.push_back(encodedVal); // Add to dataset1
        }
    }


    //==============================[Encrypt 32 bit Signed int]=======================================
    // Encrypt each value in the input data vector
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& value : oneHotData) {
        FheUint10* ciphertext = nullptr;
        ok                    = fhe_uint10_try_encrypt_with_client_key_u16(value.tenBitUInt, client_key, &ciphertext);
        assert(ok == 0);
        encrypted_data.push_back(ciphertext);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(
        start,
        end,
        "Zama Encrypt 8190 bits (819 10 digits values, 10 digits encoded into 10 bit one hot, 32 bit unsigned starting "
        "values)");
    std::cout << std::flush;


    //==============================[zama or]=======================================

    start = std::chrono::high_resolution_clock::now();

    size_t n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {
        FheUint10* result = nullptr;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_uint10_bitor(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        or_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(
        start,
        end,
        "Zama bitwise or 8190 bits (819 10 digits values, 10 digits encoded into 10 bit one hot, 32 bit unsigned "
        "starting values)");
    std::cout << std::flush;


    //==============================[zama and]=======================================

    start = std::chrono::high_resolution_clock::now();

    n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {
        FheUint10* result = nullptr;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_uint10_bitand(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        and_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(
        start,
        end,
        "Zama bitwise and 8190 bits (819 10 digits values, 10 digits encoded into 10 bit one hot, 32 bit unsigned "
        "starting values)");
    std::cout << std::flush;


    //==============================[Decrypt Added 32 bit Signed Int Cipher to Cipher]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<oneHotEncodeing::tenBitUInt> decrypted_results_or;
    start = std::chrono::high_resolution_clock::now();
    for(auto* result : or_result_data) {
        uint16_t decrypted_value;
        ok = fhe_uint10_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        auto val = oneHotEncodeing::tenBitUInt{decrypted_value};
        decrypted_results_or.push_back(val);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt or data");
    std::cout << std::flush;


    //==============================[Decrypt Added 32 bit Signed Int Cipher to Cipher]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<oneHotEncodeing::tenBitUInt> decrypted_results_and;
    start = std::chrono::high_resolution_clock::now();
    for(auto* result : and_result_data) {
        uint16_t decrypted_value;
        ok = fhe_uint10_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        auto val = oneHotEncodeing::tenBitUInt{decrypted_value};
        decrypted_results_and.push_back(val);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt or data");
    std::cout << std::flush;

    //=============================================================================================
    // End Tests
    //=============================================================================================


    //==============================[Cleanup]=======================================

    std::cout << "Data Free" << std::endl;
    // Clean up ciphertexts and keys
    for(auto* encrypted_value : encrypted_data) {
        fhe_uint10_destroy(encrypted_value);
    }
    for(auto* result : or_result_data) {
        fhe_uint10_destroy(result);
    }
    for(auto* result : and_result_data) {
        fhe_uint10_destroy(result);
    }

    std::cout << "Key Free" << std::endl;
    client_key_destroy(client_key);
    server_key_destroy(server_key);
}
