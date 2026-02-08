
#include <zama.hpp>
#include <testTimer.hpp>

#include "tfhe.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <assert.h>

// // Function to encrypt a 64-bit signed integer
// void encrypt_int64_t(LweSample **ciphertext, int64_t value, TFheGateBootstrappingSecretKeySet *secret_key) {
//     // Convert the 64-bit signed integer to binary
//     uint8_t binary[64];
//     for (int i = 0; i < 64; i++) {
//         binary[i] = (value >> (63 - i)) & 1;
//     }

//     // Encrypt each bit using the secret key
//     for (int i = 0; i < 64; i++) {
//         bootsSymEncrypt(&ciphertext[i][0], binary[i], secret_key);
//     }
// }

// // Function to decrypt a 64-bit signed integer from a ciphertext
// int64_t decrypt_int64_t(LweSample *ciphertext, TFheGateBootstrappingSecretKeySet *secret_key) {
//     int64_t value = 0;

//     // Decrypt each bit and build the 64-bit integer
//     for (int i = 0; i < 64; i++) {
//         int bit = bootsSymDecrypt(&ciphertext[i], secret_key);
//         value |= (bit << (63 - i));
//     }

//     return value;
// }

// // Function to perform homomorphic addition on encrypted ciphertexts and return the result as ciphertext
// std::vector<LweSample*> homomorphic_addition(LweSample **encrypted_input, size_t length, TFheGateBootstrappingCloudKeySet *public_key) {
//     std::vector<LweSample*> result_vector(length / 2);

//     // Perform homomorphic addition on encrypted pairs and store in result_vector
//     for (size_t i = 0; i < length / 2; i++) {
//         result_vector[i] = new_gate_bootstrapping_ciphertext_array(64, public_key->params);

//         // Perform homomorphic addition on the ciphertexts
//         for (int j = 0; j < 64; j++) {
//             bootsADD(&result_vector[i][j], &encrypted_input[2*i][j], &encrypted_input[2*i + 1][j], public_key);
//         }
//     }

//     return result_vector; // Return the result vector containing ciphertexts
// }

// // Main function to perform encryption, addition, and decryption
// void encrypt_add_decrypt(TFheGateBootstrappingSecretKeySet *secret_key, TFheGateBootstrappingCloudKeySet *public_key,
//                          const std::vector<int64_t> &input_vector) {
//     size_t length = input_vector.size();

//     // Step 1: Encrypt the vector of 64-bit signed integers

//     // Sample function you want to time
//     auto start = std::chrono::high_resolution_clock::now();
//     std::vector<LweSample*> encrypted_input(length);
//     for (size_t i = 0; i < length; i++) {
//         encrypted_input[i] = new_gate_bootstrapping_ciphertext_array(64, public_key->params);
//         encrypt_int64_t(encrypted_input[i], input_vector[i], secret_key);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     printTimingResults(start, end, "Zama Encrypt 1m 64bit signed int plaintext");

//     // Step 2: Perform homomorphic addition on encrypted pairs and return the result
//     start = std::chrono::high_resolution_clock::now();
//     std::vector<LweSample*> result_vector = homomorphic_addition(encrypted_input, length, public_key);
//     end = std::chrono::high_resolution_clock::now();
//     printTimingResults(start, end, "Zama Add Odd Even Pair 500k 64bit signed int ciphertext");

//     // Step 3: Decrypt the result vector
//     start = std::chrono::high_resolution_clock::now();
//     std::vector<int64_t> decrypted_results(result_vector.size());
//     for (size_t i = 0; i < result_vector.size(); i++) {
//         decrypted_results[i] = decrypt_int64_t(result_vector[i], secret_key);
//     }
//     end = std::chrono::high_resolution_clock::now();
//     printTimingResults(start, end, "Zama Decrypt 500k 64bit signed int ciphertext");

//     // Step 4: Print the decrypted results vector
//     std::cout << "Decrypted result vector: ";
//     for (const auto& result : decrypted_results) {
//         std::cout << result << " ";
//     }
//     std::cout << std::endl();

//     // Free resources
//     for (size_t i = 0; i < length; i++) {
//         delete_gate_bootstrapping_ciphertext_array(64, encrypted_input[i]);
//     }
//     for (size_t i = 0; i < result_vector.size(); i++) {
//         delete_gate_bootstrapping_ciphertext_array(64, result_vector[i]);
//     }
// }


// Main driver function
void zama_test_driver() {
    // Example setup and initialization (you would have your own key generation here)
    // const std::vector<int64_t> input_vector = {10, 20, 30, 40, 50, 60, 70, 80};  // Input vector
    // size_t length = input_vector.size();
    // // Generate the keys (secret key and public key)
    // TFheGateBootstrappingSecretKeySet *secret_key = new_random_gate_bootstrapping_secret_keyset(110);  // 110 is a parameter for security level
    // TFheGateBootstrappingCloudKeySet *public_key = secret_key->cloud_key;

    // // Perform the encrypt-add-decrypt process
    // encrypt_add_decrypt(secret_key, public_key, input_vector);

    // // Cleanup
    // delete_gate_bootstrapping_secret_keyset(secret_key);

    int ok = 0;
    // Prepare the config builder for the high level API and choose which types to enable
    ConfigBuilder *builder;
    Config *config;

    // Put the builder in a default state without any types enabled
    config_builder_default(&builder);
    // Populate the config
    config_builder_build(builder, &config);

    ClientKey *client_key = NULL;
    ServerKey *server_key = NULL;

    // Generate the keys using the config
    generate_keys(config, &client_key, &server_key);
    // Set the server key for the current thread
    set_server_key(server_key);

    FheUint128 *lhs = NULL;
    FheUint128 *rhs = NULL;
    FheUint128 *result = NULL;
    // A 128-bit unsigned integer containing value: 20 << 64 | 10
    U128 clear_lhs = { .w0 = 10, .w1 = 20 };
    // A 128-bit unsigned integer containing value: 2 << 64 | 1
    U128 clear_rhs = { .w0 = 1, .w1 = 2 };

    ok = fhe_uint128_try_encrypt_with_client_key_u128(clear_lhs, client_key, &lhs);
    assert(ok == 0);

    ok = fhe_uint128_try_encrypt_with_client_key_u128(clear_rhs, client_key, &rhs);
    assert(ok == 0);

    // Compute the subtraction
    ok = fhe_uint128_sub(lhs, rhs, &result);
    assert(ok == 0);

    U128 clear_result;
    // Decrypt
    ok = fhe_uint128_decrypt(result, client_key, &clear_result);
    assert(ok == 0);

    // Here the subtraction allows us to compare each word
    assert(clear_result.w0 == 9);
    assert(clear_result.w1 == 18);

    // Destroy the ciphertexts
    fhe_uint128_destroy(lhs);
    fhe_uint128_destroy(rhs);
    fhe_uint128_destroy(result);

    // Destroy the keys
    client_key_destroy(client_key);
    server_key_destroy(server_key);

    printf("FHE computation successful!\n");

}
