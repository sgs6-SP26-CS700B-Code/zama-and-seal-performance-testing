
#include <zama.hpp>
#include <testTimer.hpp>

#include "tfhe.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <assert.h>

void zama_test_driver(const std::vector<int64_t>& data)
{

    int ok = 0;

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
    std::vector<FheInt64*> encrypted_data;
    std::vector<FheInt64*> add_result_data;
    std::vector<FheInt64*> sub_result_data;
    std::vector<FheInt64*> mult_result_data;
    std::vector<FheInt64*> scalar_add_result_data;
    std::vector<FheInt64*> scalar_sub_result_data;
    std::vector<FheInt64*> scalar_mult_result_data;

    std::cout << "Begining zama tests" << std::endl;
    std::cout << std::flush;
    //=============================================================================================
    // Begin Tests
    //=============================================================================================



    //==============================[Encrypt 64 bit Signed int]=======================================
    // Encrypt each value in the input data vector
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& value : data) {
        FheInt64* ciphertext = NULL;
        ok                   = fhe_int64_try_encrypt_with_client_key_i64(value, client_key, &ciphertext);
        assert(ok == 0);
        encrypted_data.push_back(ciphertext);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama Encrypt 32bit in 64bit space signed int");
    std::cout << std::flush;


    //==============================[Add 64 bit Signed Int Cipher to Cipher]=======================================

    start = std::chrono::high_resolution_clock::now();

    size_t n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_int64_add(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        add_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama add 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[add 64 bit Signed Int Cipher to Plain]=======================================
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;
        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok               = fhe_int64_scalar_add(encrypted_data[i], data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        scalar_add_result_data.push_back(result);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama add 32bit in 64bit space signed int Cipher Scalar");
    std::cout << std::flush;

    //==============================[Sub 64 bit Signed Int Cipher to Cipher]=======================================

    start = std::chrono::high_resolution_clock::now();

    n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_int64_sub(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        sub_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama sub 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[sub 64 bit Signed Int Cipher to Plain]=======================================
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;
        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok               = fhe_int64_scalar_sub(encrypted_data[i], data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        scalar_sub_result_data.push_back(result);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama sub 32bit in 64bit space signed int Cipher Scalar");
    std::cout << std::flush;

    //==============================[Mult 64 bit Signed Int Cipher to Cipher]=======================================

    start = std::chrono::high_resolution_clock::now();

    n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_int64_mul(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        mult_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama mult 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[mult 64 bit Signed Int Cipher to Plain]=======================================
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt64*
    for(size_t i = 0; i < n; ++i) {
        FheInt64* result = NULL;
        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok               = fhe_int64_scalar_mul(encrypted_data[i], data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        scalar_mult_result_data.push_back(result);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama mult 32bit in 64bit space signed int Cipher Scalar");
    std::cout << std::flush;

    //==============================[Decrypt Added 64 bit Signed Int Cipher to Cipher]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_add;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : add_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_add.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt added 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[Decrypt Added 64 bit Signed Int Cipher to Scalar]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_add_scalar;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : scalar_add_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_add_scalar.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt added 32bit in 64bit space signed int Cipher Scalar");
    std::cout << std::flush;


    //==============================[Decrypt Subtracted 64 bit Signed Int Cipher to Cipher]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_sub;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : sub_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_sub.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt sub 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[Decrypt Subtracted 64 bit Signed Int Cipher to Scalar]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_sub_scalar;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : scalar_sub_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_sub_scalar.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt subtracted 32bit in 64bit space signed int Cipher Scalar");
    std::cout << std::flush;

    //==============================[Decrypt Multiplied 64 bit Signed Int Cipher to Cipher]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_mult;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : sub_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_mult.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt mult 32bit in 64bit space signed int Cipher Cipher");
    std::cout << std::flush;


    //==============================[Decrypt Mult 64 bit Signed Int Cipher to Scalar]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<int64_t> decrypted_results_mult_scalar;
    start = std::chrono::high_resolution_clock::now();
    for(FheInt64* result : scalar_mult_result_data) {
        int64_t decrypted_value;
        ok = fhe_int64_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_mult_scalar.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt multiplied 32bit in 64bit space space signed int Cipher Scalar");
    std::cout << std::flush;

    //=============================================================================================
    // End Tests
    //=============================================================================================


    //==============================[Assert Accuracy]=======================================

    // std::cout << "Test Comparision" << std::endl;
    // // test the results
    // n = encrypted_data.size();  // Assuming encrypted_data is a vector of FheInt64*
    // for (int i = 0; i<=data.size(); i++) {
    //     std::cout << "Cipher Cipher" << std::endl;
    //     std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << decrypted_results_add[i] << std::endl;
    //     std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << decrypted_results_sub[i] << std::endl;
    //     // std::cout <<  data[i] << " * "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult[i] << std::endl;
    //     std::cout << "Cipher Scalar" << std::endl;
    //     std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << decrypted_results_add_scalar[i] << std::endl;
    //     std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << decrypted_results_sub_scalar[i] << std::endl;
    //     // std::cout <<  data[i] << " + "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult_scalar[i] << std::endl;
    // }

    //==============================[Cleanup]=======================================

    std::cout << "Data Free" << std::endl;
    // Clean up ciphertexts and keys
    for(FheInt64* encrypted_value : encrypted_data) {
        fhe_int64_destroy(encrypted_value);
    }
    for(FheInt64* result : add_result_data) {
        fhe_int64_destroy(result);
    }
    for(FheInt64* result : sub_result_data) {
        fhe_int64_destroy(result);
    }
    for(FheInt64* result : mult_result_data) {
        fhe_int64_destroy(result);
    }

    for(FheInt64* result : scalar_add_result_data) {
        fhe_int64_destroy(result);
    }
    for(FheInt64* result : scalar_sub_result_data) {
        fhe_int64_destroy(result);
    }
    for(FheInt64* result : scalar_mult_result_data) {
        fhe_int64_destroy(result);
    }

    std::cout << "Key Free" << std::endl;
    client_key_destroy(client_key);
    server_key_destroy(server_key);
}