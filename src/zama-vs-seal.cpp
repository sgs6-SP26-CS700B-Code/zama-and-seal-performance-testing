
#include <ostream>
#include <zama-vs-seal.hpp>
#include <testTimer.hpp>

#include "tfhe.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <assert.h>

#include "seal/plaintext.h"
#include "testTimer.hpp"
#include <seal/seal.h>
#include <iostream>

using namespace std;

inline void print_parameters(const seal::SEALContext& context)
{
    auto& context_data = *context.key_context_data();

    /*
    Which scheme are we using?
    */
    std::string scheme_name;
    switch(context_data.parms().scheme()) {
        case seal::scheme_type::bfv:
            scheme_name = "BFV";
            break;
        case seal::scheme_type::ckks:
            scheme_name = "CKKS";
            break;
        case seal::scheme_type::bgv:
            scheme_name = "BGV";
            break;
        default:
            throw std::invalid_argument("unsupported scheme");
    }
    std::cout << "/" << std::endl;
    std::cout << "| Encryption parameters :" << std::endl;
    std::cout << "|   scheme: " << scheme_name << std::endl;
    std::cout << "|   poly_modulus_degree: " << context_data.parms().poly_modulus_degree() << std::endl;

    /*
    Print the size of the true (product) coefficient modulus.
    */
    std::cout << "|   coeff_modulus size: ";
    std::cout << context_data.total_coeff_modulus_bit_count() << " (";
    auto        coeff_modulus      = context_data.parms().coeff_modulus();
    std::size_t coeff_modulus_size = coeff_modulus.size();
    for(std::size_t i = 0; i < coeff_modulus_size - 1; i++) {
        std::cout << coeff_modulus[i].bit_count() << " + ";
    }
    std::cout << coeff_modulus.back().bit_count();
    std::cout << ") bits" << std::endl;

    /*
    For the BFV scheme print the plain_modulus parameter.
    */
    if(context_data.parms().scheme() == seal::scheme_type::bfv) {
        std::cout << "|   plain_modulus: " << context_data.parms().plain_modulus().value() << std::endl;
    }

    std::cout << "\\" << std::endl;
}

void print_first_ten(const std::vector<uint32_t>& data)
{

    std::cout << "[ ";

    const std::size_t count = std::min<std::size_t>(10, data.size());

    for(std::size_t i = 0; i < count; ++i) {
        std::cout << data[i] << ", ";
    }

    if(data.size() > 10) {
        std::cout << "..., ";
    }

    std::cout << "]\n";
}


void print_offset(const std::vector<uint32_t>& data)
{

    std::cout << "[ ";

    const std::size_t count = std::min<std::size_t>(10, data.size());

    for(std::size_t i = 0; i < count; ++i) {
        std::cout << data[(i + 1) % data.size()] << ", ";
    }

    if(data.size() > 10) {
        std::cout << "..., ";
    }

    std::cout << "]\n";
}


// Function to serialize and check the size
void serialize_and_check_size(const FheUint32* ciphertext)
{
    DynamicBuffer buffer;
    uint64_t      serialized_size_limit = 1024 * 1024; // Set a reasonable size limit (e.g., 1MB)

    // Call fhe_uint32_safe_serialize to serialize the ciphertext
    int result = fhe_uint32_safe_serialize(ciphertext, &buffer, serialized_size_limit);

    if(result != 0) {
        std::cerr << "Serialization failed!" << std::endl;
        return;
    }

    // Check the size of the serialized data
    std::cout << "Serialized data size: " << buffer.length << " bytes" << std::endl;

    // Don't forget to free or clean up the buffer if necessary
    if(buffer.destructor != nullptr) {
        buffer.destructor(buffer.pointer, buffer.length);
    }
}

void zama_test_serial_compute(const std::vector<uint32_t>& data)
{
    cout << "-------------------------------------------------------------\n";
    cout << "Performing Zama Test: \nDataset A: " << endl;
    print_first_ten(data);
    cout << endl;
    cout << "Dataset B: " << endl;
    print_offset(data);
    cout << endl;


    int ok = 0;
    std::cout << ok << std::endl;

    // Prepare the config builder for the high level API and choose which types to enable
    ConfigBuilder* builder;
    Config*        config;

    //     // Default Parameters (I think?)
    //     ShortintPBSParameters default_params;
    //     // fields im kinda guessing to fill out.
    //     default_params.lwe_noise_distribution.tag = 0;  // Tag for Gaussian distribution
    //     default_params.lwe_noise_distribution.distribution.gaussian.std = 1.0;  // Default std (e.g., 1.0)
    //     default_params.glwe_noise_distribution.tag = 0;  // Tag for Gaussian distribution
    //     default_params.glwe_noise_distribution.distribution.gaussian.std = 1.0;  // Default std (e.g., 1.0)
    //     default_params.modulus_switch_noise_reduction_params.tag = 0;

    // // Populate the ModulusSwitchNoiseReductionParams with default values
    //     default_params.modulus_switch_noise_reduction_params.modulus_switch_noise_reduction_params.modulus_switch_zeros_count = 0;  // Default value
    //     default_params.modulus_switch_noise_reduction_params.modulus_switch_noise_reduction_params.ms_bound = 1.0;  // Default value
    //     default_params.modulus_switch_noise_reduction_params.modulus_switch_noise_reduction_params.ms_r_sigma_factor = 1.0;  // Default value
    //     default_params.modulus_switch_noise_reduction_params.modulus_switch_noise_reduction_params.ms_input_variance = 1.0;  // Default value

    //     default_params.lwe_dimension               = 1024;    // Default moderate security level
    //     default_params.glwe_dimension              = 2;       // GLWE dimension
    //     default_params.polynomial_size             = 8192;    // Default polynomial degree is 16384, reduced this
    //     default_params.pbs_base_log                = 10;      // Modulus switching base log (default)
    //     default_params.pbs_level                   = 2;       // Modulus switching depth
    //     default_params.ks_base_log                 = 10;      // Key switching base log
    //     default_params.ks_level                    = 2;       // Key switching depth
    //     default_params.message_modulus             = 100000;  // Modulus for the message
    //     default_params.carry_modulus               = 1000000; // Modulus for carry
    //     default_params.max_noise_level             = 2;       // Max noise tolerance
    //     default_params.log2_p_fail                 = -50;     // Probability of failure, default = -60?
    //     default_params.modulus_power_of_2_exponent = 12;      // Modulus size (2^14) (default 14, reduced)
    //     default_params.encryption_key_choice       = ShortintEncryptionKeyChoiceSmall; // Default key type

    // config_builder_default(&builder);

    // // Use the default parameters
    // config_builder_use_custom_parameters(&builder, default_params);

    // // Build the config object
    // config_builder_build(builder, &config);


    // // Uncomment this and comment above if you want true default config.
    // config_builder_default(&builder);
    // // Populate the config
    // config_builder_build(builder, &config);

    config_builder_default(&builder);
    config_builder_use_custom_parameters(&builder, SHORTINT_V1_5_PARAM_MESSAGE_2_CARRY_2_PBS_KS_GAUSSIAN_2M128);
    config_builder_build(builder, &config);

    ClientKey* client_key = nullptr;
    ServerKey* server_key = nullptr;

    // Generate the keys using the config
    generate_keys(config, &client_key, &server_key);
    // Set the server key for the current thread
    set_server_key(server_key);

    // Create vectors to hold the encrypted values
    std::vector<FheUint32*> encrypted_data;
    std::vector<FheUint32*> add_result_data;
    std::vector<FheUint32*> mult_result_data;

    std::cout << "Begining zama tests" << std::endl;
    std::cout << std::flush;
    //=============================================================================================
    // Begin Tests
    //=============================================================================================



    //==============================[Encrypt 32 bit Signed int]=======================================
    // Encrypt each value in the input data vector
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& value : data) {
        FheUint32* ciphertext = nullptr;
        ok                    = fhe_uint32_try_encrypt_with_client_key_u32(value, client_key, &ciphertext);
        assert(ok == 0);
        encrypted_data.push_back(ciphertext);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama Encrypt 32bit in 32bit space signed int");
    std::cout << std::flush;

    cout << "\nZama ciphertext first value attempt to check size?\n";
    serialize_and_check_size(encrypted_data[0]);


    //==============================[Add 32 bit ]=======================================

    start = std::chrono::high_resolution_clock::now();

    size_t n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheUint32*
    for(size_t i = 0; i < n; ++i) {
        FheUint32* result = nullptr;

        // Determine the next index, wrapping around to the first element when reaching the last element

        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_uint32_add(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        add_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama add 32 bit uint (cipher+cipher)");
    std::cout << std::flush;

    cout << "\nZama added ciphertext first value attempt to check size?\n";
    serialize_and_check_size(add_result_data[0]);

    //==============================[add 32 bit Signed Int Cipher to Plain]=======================================
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheUint32*
    for(size_t i = 0; i < n; ++i) {
        FheUint32* result = nullptr;

        // Determine the next index, wrapping around to the first element when reaching the last element
        // Perform the homomorphic subtraction (encrypted_value[i] - encrypted_value[i + 1])
        ok = fhe_uint32_mul(encrypted_data[i], encrypted_data[(i + 1) % n], &result);
        assert(ok == 0);

        // Store the result of the subtraction
        mult_result_data.push_back(result);
    }

    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama mult 32bit uint (cipher*cipher)");
    std::cout << std::flush;

    cout << "\nZama multiplied ciphertext first value attempt to check size?\n";
    serialize_and_check_size(mult_result_data[0]);

    //==============================[Decrypt Added]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<uint32_t> decrypted_results_add;
    start = std::chrono::high_resolution_clock::now();
    for(FheUint32* result : add_result_data) {
        uint32_t decrypted_value;
        ok = fhe_uint32_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_add.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt addition");
    std::cout << std::flush;


    //==============================[Decrypt Multiplication]=======================================

    // Decrypt the results and store them in a new vector
    std::vector<uint32_t> decrypted_results_mult;
    start = std::chrono::high_resolution_clock::now();
    for(FheUint32* result : mult_result_data) {
        uint32_t decrypted_value;
        ok = fhe_uint32_decrypt(result, client_key, &decrypted_value);
        assert(ok == 0);
        decrypted_results_mult.push_back(decrypted_value);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Zama decrypt multiplication");
    std::cout << std::flush;



    //=============================================================================================
    // End Tests
    //=============================================================================================

    cout << "Zama Final Results (Addition): \n";
    cout << "A: " << endl;
    print_first_ten(data);
    cout << "+" << endl;
    cout << "B: " << endl;
    print_offset(data);
    cout << "=" << endl;
    print_first_ten(decrypted_results_add);
    cout << endl << endl;

    cout << "Zama Final Results (multiplication): \n";
    cout << "A: " << endl;
    print_first_ten(data);
    cout << "*" << endl;
    cout << "B: " << endl;
    print_offset(data);
    cout << "=" << endl;
    print_first_ten(decrypted_results_mult);

    //==============================[Cleanup]=======================================

    std::cout << "Data Free" << std::endl;
    // Clean up ciphertexts and keys
    for(auto* encrypted_value : encrypted_data) {
        fhe_uint32_destroy(encrypted_value);
    }
    for(auto* result : add_result_data) {
        fhe_uint32_destroy(result);
    }
    for(auto* result : mult_result_data) {
        fhe_uint32_destroy(result);
    }


    std::cout << "Key Free" << std::endl;
    client_key_destroy(client_key);
    server_key_destroy(server_key);
}

std::vector<uint32_t> flatten_and_append(const std::vector<std::vector<uint64_t>>& nested_vec)
{
    std::vector<uint32_t> result;

    // Iterate over each child vector in the outer vector
    for(const auto& child : nested_vec) {
        // Append each element from the child vector to the result
        for(const auto& value : child) {
            result.push_back(value); // Add the value to the result
        }
    }

    return result;
}

void seal_test_16384_batch_size(const std::vector<uint32_t>& data)
{
    using namespace seal;

    cout << "-------------------------------------------------------------\n";
    cout << "Performing SEAL 16384 batch Test: \nDataset A: " << endl;
    print_first_ten(data);
    cout << endl;
    cout << "Dataset B: " << endl;
    print_offset(data);
    cout << endl;


    // Get the size of the vector in terms of elements
    size_t num_elements = data.size();

    // Size of each element in the vector (uint32_t is 4 bytes)
    size_t element_size = sizeof(uint32_t);

    // Calculate the size of the vector's storage (including its capacity)
    size_t total_size = num_elements * element_size + sizeof(data);

    std::cout << "Size of vector storage (in bytes): " << total_size << std::endl;

    // You can also check the size of the internal data buffer
    std::cout << "Capacity in elements: " << data.capacity() << std::endl;
    std::cout << "Size of the data buffer in bytes: " << data.capacity() * element_size << std::endl;

    //==============================[Parameter Setup]=======================================
    EncryptionParameters parms(scheme_type::bgv);
    size_t               poly_modulus_degree =
        16384; // It seems this being lowered helps with the mixed signed cases... but that lowers security?
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 40));
    SEALContext context(parms);
    print_parameters(context);
    cout << "Parameter validation (success): " << context.parameter_error_message() << endl;

    //==============================[Key Setup]=======================================
    KeyGenerator     keygen(context);
    const SecretKey& secret_key = keygen.secret_key();
    seal::PublicKey  public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);

    //==============================[Function Setup?]=======================================
    BatchEncoder batch_encoder(context);
    Encryptor    encryptor(context, public_key);
    Evaluator    evaluator(context);
    Decryptor    decryptor(context, secret_key);

    //=============================================================================================
    // Begin Tests
    //=============================================================================================

    size_t slot_count = batch_encoder.slot_count();
    cout << "Batching Slot Count = " << slot_count << endl;

    //==============================[Encode plaintext set a in 16384 size batches]=======================================
    // encode each value in the input data vector

    vector<Plaintext> encoded_values_a{};
    auto              start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < data.size(); i++) {
        // Prepare the batch and the corresponding plaintext object
        Plaintext        encoded_batched_plaintext_data_a;
        vector<uint64_t> batch(batch_encoder.slot_count(), 0);

        // Process the batch: maximum 16384 slots per batch
        for(int j = 0; j < 16384 && i < data.size(); j++) {
            batch[j] = data[i]; // Use j as the index for batch
            i++;                // Move to the next element in data
        }

        // Encode the current batch
        batch_encoder.encode(batch, encoded_batched_plaintext_data_a);

        // Add the encoded data to the result vector
        encoded_values_a.push_back(encoded_batched_plaintext_data_a); // Push the encoded batch


        // If the batch was less than 16384 elements, we should stop.
        // The outer loop will continue to process any remaining data.
        if(i >= data.size()) {
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv batch encode dataset a");
    std::cout << std::flush;

    long size = 0;
    for(const auto& value : encoded_values_a) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of batched plaintext vector A: " << size << std::endl << std::flush;


    vector<Plaintext> encoded_values_b{};
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < data.size(); i++) {
        // Prepare the batch and the corresponding plaintext object
        Plaintext        encoded_batched_plaintext_data_b;
        vector<uint64_t> batch(batch_encoder.slot_count(), 0);

        // Process the batch: maximum 16384 slots per batch
        for(int j = 0; j < 16384 && i < data.size(); j++) {
            batch[j] = data[(i + 1) % data.size()]; // Use j as the index for batch
            i++;                                    // Move to the next element in data
        }

        // Encode the current batch
        batch_encoder.encode(batch, encoded_batched_plaintext_data_b);

        // Add the encoded data to the result vector
        encoded_values_b.push_back(encoded_batched_plaintext_data_b); // Push the encoded batch

        // If the batch was less than 16384 elements, we should stop.
        // The outer loop will continue to process any remaining data.
        if(i >= data.size()) {
            break;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv batch encode dataset b");
    std::cout << std::flush;

    size = 0;
    for(const auto& value : encoded_values_b) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of batched plaintext vector B: " << size << std::endl << std::flush;

    //==============================[encrypt encoded batch a]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<Ciphertext> encrypted_data_a;
    for(const auto& batch : encoded_values_a) {
        Ciphertext encrypted_data;
        encryptor.encrypt(batch, encrypted_data);
        encrypted_data_a.push_back(encrypted_data);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv encrypt all batches in set A");
    std::cout << std::flush;

    size = 0;
    for(const auto& value : encrypted_data_a) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of ciphertext vector A: " << size << std::endl << std::flush;


    //==============================[encrypt encoded batch b]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<Ciphertext> encrypted_data_b;
    for(const auto& batch : encoded_values_b) {
        Ciphertext encrypted_data;
        encryptor.encrypt(batch, encrypted_data);
        encrypted_data_b.push_back(encrypted_data);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv encrypt all batches in set B");
    std::cout << std::flush;

    size = 0;
    for(const auto& value : encrypted_data_b) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of ciphertext vector B: " << size << std::endl << std::flush;

    //==============================[Add batch 32 bit Cipher Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<Ciphertext> a_plus_b_batch;
    for(int i = 0; i < encoded_values_a.size(); i++) {
        Ciphertext encrypted_chunk;
        evaluator.add(encrypted_data_a[i], encrypted_data_b[i], encrypted_chunk);
        a_plus_b_batch.push_back(encrypted_chunk);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv add two batch 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;


    size = 0;
    for(const auto& value : a_plus_b_batch) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of added ciphertext vector a_plus_b_batch: " << size << std::endl
         << std::flush;

    //==============================[mult batch 32 bit Cipher Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<Ciphertext> a_mul_b_batch;
    for(int i = 0; i < encoded_values_a.size(); i++) {
        Ciphertext encrypted_chunk;
        evaluator.multiply(encrypted_data_a[i], encrypted_data_b[i], encrypted_chunk);
        a_mul_b_batch.push_back(encrypted_chunk);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv multiply two batch 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;


    size = 0;
    for(const auto& value : a_mul_b_batch) {
        size += value.save_size();
    }
    cout << "\n Upper Bound size in bytes of multiplied ciphertext vector a_mul_b_batch: " << size << std::endl
         << std::flush;


    //==============================[decrypt & decode added ciphertext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<vector<uint64_t>> decoded_values_add{};
    vector<Plaintext>        plain_encoded_add;
    for(int i = 0; i < a_plus_b_batch.size(); i++) {
        Plaintext plain_encoded;
        decryptor.decrypt(a_plus_b_batch[i], plain_encoded);
        vector<uint64_t> decoded_batch(batch_encoder.slot_count(), 0);
        batch_encoder.decode(plain_encoded, decoded_batch);
        decoded_values_add.push_back(decoded_batch);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv decrypt and decode batched 32 bit add");
    std::cout << std::flush;


    //==============================[decrypt & decode subtracted ciphertext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<vector<uint64_t>> decoded_values_mul{};
    vector<Plaintext>        plain_encoded_mul;
    for(int i = 0; i < a_mul_b_batch.size(); i++) {
        Plaintext plain_encoded;
        decryptor.decrypt(a_mul_b_batch[i], plain_encoded);
        vector<uint64_t> decoded_batch(batch_encoder.slot_count(), 0);
        batch_encoder.decode(plain_encoded, decoded_batch);
        decoded_values_mul.push_back(decoded_batch);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bgv decrypt and decode batched 32 bit mult");
    std::cout << std::flush;

    auto addResult = flatten_and_append(decoded_values_add);
    auto mulResult = flatten_and_append(decoded_values_mul);


    cout << "SEAL Final Results (Addition): \n";
    cout << "A: " << endl;
    print_first_ten(data);
    cout << "+" << endl;
    cout << "B: " << endl;
    print_offset(data);
    cout << "=" << endl;
    print_first_ten(addResult);
    cout << endl << endl;

    cout << "SEAL Final Results (multiplication): \n";
    cout << "A: " << endl;
    print_first_ten(data);
    cout << "*" << endl;
    cout << "B: " << endl;
    print_offset(data);
    cout << "=" << endl;
    print_first_ten(mulResult);
}