
#include <cstdint>
#include <sealBfvBatchOneHot.hpp>
#include <seal/seal.h>
#include "oneHotEncoding.hpp"
#include "seal/plaintext.h"
#include "testTimer.hpp"
#include <iomanip>
#include <seal/seal.h>
#include <array>
#include <iostream>

using namespace std;
using namespace seal;

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


void seal_bfv_one_hot_encode_32_batch_8192(const std::vector<uint32_t>& data){
 //==============================[Parameter Setup]=======================================
    EncryptionParameters parms(scheme_type::bfv);
    size_t               poly_modulus_degree =
        8192;
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 60));
    SEALContext context(parms);
    print_parameters(context);
    cout << "Parameter validation (success): " << context.parameter_error_message() << endl;

    //==============================[Key Setup]=======================================
    KeyGenerator     keygen(context);
    const SecretKey& secret_key = keygen.secret_key();
    PublicKey        public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);

    //==============================[Function Setup?]=======================================
    BatchEncoder batch_encoder(context);
    Encryptor    encryptor(context, public_key);
    Evaluator    evaluator(context);
    Decryptor    decryptor(context, secret_key);

    // Prep Data

    vector<array<uint32_t, 10>> data_one_hot{};
    for (auto val : data){
        auto vec = oneHotEncodeing::oneHotEncodeUInt32(val);
        int vectorSize = vec.size();
        array<uint32_t, 10> arr{};
        // Fill the array with values from the vector, starting from the end
        for (int i = 0; i < vectorSize; ++i) {
            arr[9 - i] = vec.at(vectorSize - 1 - i);  // Fill from array[9] to array[9 - (n-1)]
        }
        data_one_hot.push_back(arr);
    }


    //=============================================================================================
    // Begin Tests
    //=============================================================================================

    size_t slot_count = batch_encoder.slot_count();
    cout << "Batching Slot Count = " << slot_count << endl;

    //==============================[encode plaintext 32 bit unsigned int a]=======================================
    // encode each value in the input data vector

    Plaintext       encoded_batched_plaintext_data_a;
    vector<uint64_t> encoded_values_a(batch_encoder.slot_count(), 0);
    auto            start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i <= 8190; i += 0) {
        int j = 0;
        for (auto oneHotNumArr : data_one_hot){
            encoded_values_a[i] = oneHotNumArr[j];
            i++;
            j++;
        }
    }
    batch_encoder.encode(encoded_values_a, encoded_batched_plaintext_data_a);
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal BFV Batch encode 8190 digits A (819 10 digits values, 32 bit unsigned values)");
    std::cout << std::flush;


    //==============================[encode plaintext 32 bit unsigned int b]=======================================
    // encode each value in the input data vector

    Plaintext       encoded_batched_plaintext_data_b;
    vector<uint64_t> encoded_values_b(batch_encoder.slot_count(), 0);
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i <= 8190; i += 0) {
        int j = 0;
        for (auto oneHotNumArr : data_one_hot){
            encoded_values_a[i] = oneHotNumArr[j];
            i++;
            j++;
        }
    }
    batch_encoder.encode(encoded_values_b, encoded_batched_plaintext_data_b);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal BFV Batch encode 8190 digits B (819 10 digits values, 32 bit unsigned values)");
    std::cout << std::flush;

    //==============================[encrypt encoded batch a plaintext 32 bit unsigned int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext encrypted_data_a;
    encryptor.encrypt(encoded_batched_plaintext_data_a, encrypted_data_a);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt batch a 32 bit in 64 encode");
    std::cout << std::flush;

    //==============================[encrypt encoded batch b plaintext 32 bit unsigned int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext encrypted_data_b;
    encryptor.encrypt(encoded_batched_plaintext_data_b, encrypted_data_b);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt batch b 32 bit in 64 encode");
    std::cout << std::flush;

    //==============================[Add batch 32 bit Cipher unsigned int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext a_plus_b_batch;
    evaluator.add(encrypted_data_a, encrypted_data_b, a_plus_b_batch);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv add two batch 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[mult batch 32 bit Cipher unsigned int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext a_mult_b_batch;
    evaluator.multiply(encrypted_data_a, encrypted_data_b, a_mult_b_batch);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv subtract two batch 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;


    //==============================[decrypt & decode added ciphertext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<int64_t> decoded_values_add(batch_encoder.slot_count(), 0);
    Plaintext       plain_encoded_add;
    decryptor.decrypt(a_plus_b_batch, plain_encoded_add);
    batch_encoder.decode(plain_encoded_add, decoded_values_add);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode batched 32 bit added in 64 bit vector");
    std::cout << std::flush;


    //==============================[decrypt & decode subtracted ciphertext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    vector<int64_t> decoded_values_mult(batch_encoder.slot_count(), 0);
    Plaintext       plain_encoded_mult;
    decryptor.decrypt(a_mult_b_batch, plain_encoded_mult);
    batch_encoder.decode(plain_encoded_mult, decoded_values_mult);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode batched 32 bit mult in 64 bit vector");
    std::cout << std::flush;

    //==============================[Assert Accuracy]=======================================

    /// @note Similar to the normal case, there are some values that are not happy... Below is a commented out snippet of the output.. I think there may be overflows?

    // std::cout << "Test Comparision" << std::endl;
    // // test the results
    // for (int i = 0; i<=data.size()/2; i++) {
    //     std::cout << "Cipher Cipher" << std::endl;
    //     std::cout <<  data[i] << " + "<< data[data.size()/2 + i] << " = Normal: " <<  data[i] + data[data.size()/2 + i] << " = FHE: " << decoded_values_add[i] << std::endl;
    //     std::cout <<  data[i] << " - "<< data[data.size()/2 + i] << " = Normal: " <<  data[i] - data[data.size()/2 + i] << " = FHE: " << decoded_values_sub[i] << std::endl;
    //     // std::cout <<  data[i] << " * "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult[i] << std::endl;
    //     // std::cout << "Cipher Scalar" << std::endl;
    //     // std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << add_plain_plaintext[i] << std::endl;
    //     // std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << sub_plain_plaintext[i] << std::endl;
    //     // std::cout <<  data[i] << " + "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult_scalar[i] << std::endl;
    //     std::cout << std::flush;
    // }
}