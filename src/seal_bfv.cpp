#include "seal.hpp"
#include "seal/plaintext.h"
#include "testTimer.hpp"
#include <iomanip>
#include <seal/seal.h>
#include <array>
#include <iostream>
using namespace seal;
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


void seal_bfv_test_driver_32_no_mult(const std::vector<int32_t>& data)
{
    //==============================[Parameter Setup]=======================================
    EncryptionParameters parms(scheme_type::bfv);
    size_t               poly_modulus_degree =
        4096; // It seems this being lowered helps with the mixed signed cases... but that lowers security?
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    // Prime must be 1 bit more than the bitlength of the plaintext, so bit_size is 33?
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 33));
    SEALContext context(parms);
    print_parameters(context);
    cout << "Parameter validation (success): " << context.parameter_error_message() << endl;

    //==============================[Key Setup]=======================================
    KeyGenerator     keygen(context);
    const SecretKey& secret_key = keygen.secret_key();
    PublicKey        public_key;
    keygen.create_public_key(public_key);

    //==============================[Function Setup?]=======================================
    BatchEncoder batch_encoder(context);
    Encryptor    encryptor(context, public_key);
    Evaluator    evaluator(context);
    Decryptor    decryptor(context, secret_key);

    //=============================================================================================
    // Begin Tests
    //=============================================================================================


    //==============================[encode plaintext 32 bit Signed int]=======================================
    // encode each value in the input data vector

    std::vector<Plaintext> encoded_plaintext_data;
    auto                   start = std::chrono::high_resolution_clock::now();
    for(const auto& value : data) {
        vector<int64_t> encoded_value(batch_encoder.slot_count(), 0);
        encoded_value[0] = value;
        Plaintext plain;
        batch_encoder.encode(encoded_value, plain);
        encoded_plaintext_data.push_back(plain);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encode plaintext single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[encrypt encoded plaintext 32 bit Signed int]=======================================

    std::vector<Ciphertext> encrypted_data;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encoded_plaintext_data) {
        Ciphertext encrypted;
        encryptor.encrypt(value, encrypted);
        encrypted_data.push_back(encrypted);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt ciphertext single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[Add 32 bit Cipher Signed int]=======================================

    std::vector<Ciphertext> encrypted_add_data;
    start    = std::chrono::high_resolution_clock::now();
    size_t n = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {

        Ciphertext x_plus_y;
        evaluator.add(encrypted_data[i], encrypted_data[(i + 1) % n], x_plus_y);
        encrypted_add_data.push_back(x_plus_y);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv add two 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[subtract 32 bit Cipher Signed int]=======================================

    std::vector<Ciphertext> encrypted_sub_data;
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {

        Ciphertext x_plus_y;
        evaluator.sub(encrypted_data[i], encrypted_data[(i + 1) % n], x_plus_y);
        encrypted_sub_data.push_back(x_plus_y);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv subtract two 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;


    //==============================[Add 32 bit Cipher and plain Signed int]=======================================

    std::vector<Ciphertext> encrypted_add_plain_data;
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {

        Ciphertext x_plus_y;
        evaluator.add_plain(encrypted_data[i], encoded_plaintext_data[(i + 1) % n], x_plus_y);
        encrypted_add_plain_data.push_back(x_plus_y);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv add one 32bit cipher and 1 plain in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[subtract 32 bit Cipher and plain Signed int]=======================================

    std::vector<Ciphertext> encrypted_sub_plain_data;
    start = std::chrono::high_resolution_clock::now();
    n     = encrypted_data.size(); // Assuming encrypted_data is a vector of FheInt32*
    for(size_t i = 0; i < n; ++i) {

        Ciphertext x_plus_y;
        evaluator.sub_plain(encrypted_data[i], encoded_plaintext_data[(i + 1) % n], x_plus_y);
        encrypted_sub_plain_data.push_back(x_plus_y);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv subtract one 32bit cipher and 1 plain in single slot 64bit vector");
    std::cout << std::flush;


    //==============================[decrypt & decode added ciphertext 32 bit Signed int]=======================================

    std::vector<int32_t> add_plaintext;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encrypted_add_data) {
        Plaintext plain_encoded;
        decryptor.decrypt(value, plain_encoded);
        std::vector<int64_t> plain_decoded;
        batch_encoder.decode(plain_encoded, plain_decoded);
        add_plaintext.push_back(plain_decoded[0]);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode added single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[decrypt & decode subtracted ciphertext 32 bit Signed int]=======================================

    std::vector<int32_t> sub_plaintext;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encrypted_sub_data) {
        Plaintext plain_encoded;
        decryptor.decrypt(value, plain_encoded);
        std::vector<int64_t> plain_decoded;
        batch_encoder.decode(plain_encoded, plain_decoded);
        sub_plaintext.push_back(plain_decoded[0]);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode subtracted single 32bit in single slot 64bit vector");
    std::cout << std::flush;


    //==============================[decrypt & decode added plain ciphertext 32 bit Signed int]=======================================

    std::vector<int32_t> add_plain_plaintext;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encrypted_add_plain_data) {
        Plaintext plain_encoded;
        decryptor.decrypt(value, plain_encoded);
        std::vector<int64_t> plain_decoded;
        batch_encoder.decode(plain_encoded, plain_decoded);
        add_plain_plaintext.push_back(plain_decoded[0]);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode added single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[decrypt & decode subtracted plain ciphertext 32 bit Signed int]=======================================

    std::vector<int32_t> sub_plain_plaintext;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encrypted_sub_plain_data) {
        Plaintext plain_encoded;
        decryptor.decrypt(value, plain_encoded);
        std::vector<int64_t> plain_decoded;
        batch_encoder.decode(plain_encoded, plain_decoded);
        sub_plain_plaintext.push_back(plain_decoded[0]);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode subtracted single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[Assert Accuracy]=======================================

    /// @note I have zero clue why, since things should be good, but for some reason I cant figure out how to get signed values to work properly...
    /// Anything that is unsigned +- unsigned (or well, positive +- positive), or oddly negative +- negative works fine with no issues, but anything that is a mix of positive and negative,
    /// or negative and negative, will fail to do the math accurately

    // std::cout << "Test Comparision" << std::endl;
    // // test the results
    // n = encrypted_data.size();  // Assuming encrypted_data is a vector of FheInt64*
    // for (int i = 0; i<=data.size(); i++) {
    //     std::cout << "Cipher Cipher" << std::endl;
    //     std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << add_plaintext[i] << std::endl;
    //     std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << sub_plaintext[i] << std::endl;
    //     // std::cout <<  data[i] << " * "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult[i] << std::endl;
    //     std::cout << "Cipher Scalar" << std::endl;
    //     std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << add_plain_plaintext[i] << std::endl;
    //     std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << sub_plain_plaintext[i] << std::endl;
    //     // std::cout <<  data[i] << " + "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult_scalar[i] << std::endl;
    //     std::cout << std::flush;
    // }
}

/// @todo multiplication is not accurate in any case... Not certain what is wrong, likely need param tuning
void seal_bfv_test_driver_32_fixed_mults(const std::vector<int32_t>& data)
{
    //==============================[Parameter Setup]=======================================
    EncryptionParameters parms(scheme_type::bfv);
    size_t               poly_modulus_degree =
        4096; // It seems this being lowered helps with the mixed signed cases... but that lowers security?
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    // Prime must be 1 bit more than the bitlength of the plaintext, so bit_size is 33?
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 33));
    SEALContext context(parms);
    print_parameters(context);
    cout << "Parameter validation (success): " << context.parameter_error_message() << endl;

    //==============================[Key Setup]=======================================
    KeyGenerator     keygen(context);
    const SecretKey& secret_key = keygen.secret_key();
    PublicKey        public_key;
    keygen.create_public_key(public_key);

    //==============================[Function Setup?]=======================================
    BatchEncoder batch_encoder(context);
    Encryptor    encryptor(context, public_key);
    Evaluator    evaluator(context);
    Decryptor    decryptor(context, secret_key);

    //=============================================================================================
    // Begin Tests
    //=============================================================================================


    //==============================[encode plaintext 32 bit Signed int]=======================================
    // encode each value in the input data vector

    std::vector<Plaintext> encoded_plaintext_data;
    auto                   start = std::chrono::high_resolution_clock::now();
    for(const auto& value : data) {
        vector<int64_t> encoded_value(batch_encoder.slot_count(), 0);
        encoded_value[0] = value;
        Plaintext plain;
        batch_encoder.encode(encoded_value, plain);
        encoded_plaintext_data.push_back(plain);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encode plaintext single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[encrypt encoded plaintext 32 bit Signed int]=======================================

    std::vector<Ciphertext> encrypted_data;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encoded_plaintext_data) {
        Ciphertext encrypted;
        encryptor.encrypt(value, encrypted);
        encrypted_data.push_back(encrypted);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt ciphertext single 32bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[mult 5 32 bit Cipher Signed int]=======================================

    Plaintext  plain5("5");
    Ciphertext cipher5;
    encryptor.encrypt(plain5, cipher5);
    std::vector<Ciphertext> encrypted_mult_5_data;
    start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < data.size(); ++i) {
        Ciphertext x_plus_y;
        evaluator.multiply(encrypted_data[i], cipher5, x_plus_y);
        encrypted_mult_5_data.push_back(x_plus_y);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv multiply one 32bit ciphers by 5 in single slot 64bit vector");
    std::cout << std::flush;


    //==============================[decrypt & decode subtracted ciphertext 32 bit Signed int]=======================================

    std::vector<int64_t> mult_5_plaintext;
    start = std::chrono::high_resolution_clock::now();
    for(const auto& value : encrypted_mult_5_data) {
        Plaintext plain_encoded;
        decryptor.decrypt(value, plain_encoded);
        std::vector<int64_t> plain_decoded;
        batch_encoder.decode(plain_encoded, plain_decoded);
        mult_5_plaintext.push_back(plain_decoded[0]);
    }
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode mult 5 single 39bit in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[Assert Accuracy]=======================================

    /// @note I have zero clue why, since things should be good, but for some reason I cant figure out how to get signed values to work properly...
    /// Anything that is unsigned +- unsigned (or well, positive +- positive), or oddly negative +- negative works fine with no issues, but anything that is a mix of positive and negative,
    /// or negative and negative, will fail to do the math accurately

    std::cout << "Test Comparision" << std::endl;
    // test the results
    for(int i = 0; i <= data.size(); i++) {
        std::cout << "Cipher Cipher" << std::endl;
        std::cout << data[i] << " * " << 5 << " = Normal: " << data[i] * 5 << " = FHE: " << mult_5_plaintext[i]
                  << std::endl;
        // std::cout << "Cipher Scalar" << std::endl;
        // std::cout <<  data[i] << " + "<< data[(i + 1) % n] << " = Normal: " <<  data[i] + data[(i + 1) % n] << " = FHE: " << decrypted_results_add_scalar[i] << std::endl;
        // std::cout <<  data[i] << " - "<< data[(i + 1) % n] << " = Normal: " <<  data[i] - data[(i + 1) % n] << " = FHE: " << decrypted_results_sub_scalar[i] << std::endl;
        // std::cout <<  data[i] << " + "<< data[i] << " = Normal: " <<  data[i] * data[i] << " = FHE: " << decrypted_results_mult_scalar[i] << std::endl;
        std::cout << std::flush;
    }
}


void seal_bfv_test_driver_32_no_mult_batch(const std::vector<int32_t>& data)
{
    //==============================[Parameter Setup]=======================================
    EncryptionParameters parms(scheme_type::bfv);
    size_t               poly_modulus_degree =
        32768; // It seems this being lowered helps with the mixed signed cases... but that lowers security?
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 40));
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

    //=============================================================================================
    // Begin Tests
    //=============================================================================================

    size_t slot_count = batch_encoder.slot_count();
    cout << "Batching Slot Count = " << slot_count << endl;

    //==============================[encode plaintext 32 bit Signed int a]=======================================
    // encode each value in the input data vector

    Plaintext       encoded_batched_plaintext_data_a;
    vector<int64_t> encoded_values_a(batch_encoder.slot_count(), 0);
    auto            start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < data.size(); i++) {
        encoded_values_a[i] = data[i];
    }
    batch_encoder.encode(encoded_values_a, encoded_batched_plaintext_data_a);
    auto end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv batch encode first half 32 bit dataset in 64bit encoder");
    std::cout << std::flush;


    //==============================[encode plaintext 32 bit Signed int b]=======================================
    // encode each value in the input data vector

    Plaintext       encoded_batched_plaintext_data_b;
    vector<int64_t> encoded_values_b(batch_encoder.slot_count(), 0);
    start = std::chrono::high_resolution_clock::now();
    int j = 0;
    for(int i = data.size() / 2; i < data.size(); i++) {
        encoded_values_b[j] = data[i];
        j++;
    }
    batch_encoder.encode(encoded_values_b, encoded_batched_plaintext_data_b);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv batch encode second half 32 bit dataset in 64bit encoder");
    std::cout << std::flush;

    //==============================[encrypt encoded batch a plaintext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext encrypted_data_a;
    encryptor.encrypt(encoded_batched_plaintext_data_a, encrypted_data_a);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt batch a 32 bit in 64 encode");
    std::cout << std::flush;

    //==============================[encrypt encoded batch b plaintext 32 bit Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext encrypted_data_b;
    encryptor.encrypt(encoded_batched_plaintext_data_b, encrypted_data_b);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv encrypt batch b 32 bit in 64 encode");
    std::cout << std::flush;

    //==============================[Add batch 32 bit Cipher Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext a_plus_b_batch;
    evaluator.add(encrypted_data_a, encrypted_data_b, a_plus_b_batch);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv add two batch 32bit ciphers in single slot 64bit vector");
    std::cout << std::flush;

    //==============================[subtract batch 32 bit Cipher Signed int]=======================================

    start = std::chrono::high_resolution_clock::now();
    Ciphertext a_subtract_b_batch;
    evaluator.sub(encrypted_data_a, encrypted_data_b, a_subtract_b_batch);
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
    vector<int64_t> decoded_values_sub(batch_encoder.slot_count(), 0);
    Plaintext       plain_encoded_sub;
    decryptor.decrypt(a_subtract_b_batch, plain_encoded_sub);
    batch_encoder.decode(plain_encoded_sub, decoded_values_sub);
    end = std::chrono::high_resolution_clock::now();
    printTimingResults(start, end, "Seal bfv decrypt and decode batched 32 bit subtracted in 64 bit vector");
    std::cout << std::flush;

    //==============================[Assert Accuracy]=======================================

    /// @note Similar to the normal case, there are some values that are not happy... Below is a commented out snippet of the output.. I think there may be overflows?
    // Cipher Cipher
    // -1730599776 + -1154935244 = Normal: 1409432276 = FHE: -2885535020
    // -1730599776 - -1154935244 = Normal: -575664532 = FHE: -575664532
    // Cipher Cipher
    // 1899787543 + 2078719138 = Normal: -316460615 = FHE: 3978506681
    // 1899787543 - 2078719138 = Normal: -178931595 = FHE: -178931595
    // Cipher Cipher
    // -1899903291 + -1101207377 = Normal: 1293856628 = FHE: -3001110668
    // -1899903291 - -1101207377 = Normal: -798695914 = FHE: -798695914
    // Cipher Cipher
    // 1485044360 + 1689673426 = Normal: -1120249510 = FHE: 3174717786
    // 1485044360 - 1689673426 = Normal: -204629066 = FHE: -204629066
    // Cipher Cipher
    // 297554191 + 698413480 = Normal: 995967671 = FHE: 995967671
    // 297554191 - 698413480 = Normal: -400859289 = FHE: -400859289
    // Cipher Cipher
    // 173676006 + -1233562807 = Normal: -1059886801 = FHE: -1059886801
    // 173676006 - -1233562807 = Normal: 1407238813 = FHE: 1407238813
    // Cipher Cipher
    // -59342589 + 932003336 = Normal: 872660747 = FHE: 872660747
    // -59342589 - 932003336 = Normal: -991345925 = FHE: -991345925
    // Cipher Cipher
    // 1523959301 + -301656550 = Normal: 1222302751 = FHE: 1222302751
    // 1523959301 - -301656550 = Normal: 1825615851 = FHE: 1825615851
    // Cipher Cipher
    // 287304441 + 909192458 = Normal: 1196496899 = FHE: 287304441
    // 287304441 - 909192458 = Normal: -621888017 = FHE: 287304441
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
