#include "seal.hpp"
#include <seal/seal.h>
#include <array>
#include <iostream>
using namespace seal;
using namespace std;

///@todo this method that I copied from old tests was based on testing out batching with BGV.
/// I will want to do that later, but for now this needs to be converted to normal BGV within a similar
/// test architecture to what was does for Zama, meaning I need to double check what the difference is
/// config wise for setting up batching vs setting up normal.
void seal_bgv_test_driver(const std::vector<int64_t>& data)
{

    EncryptionParameters parms(scheme_type::bfv);
    size_t               poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

    SEALContext context(parms);

    // auto qualifiers = context.first_context_data()->qualifiers();

    KeyGenerator keygen(context);
    SecretKey    secret_key = keygen.secret_key();
    PublicKey    public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    BatchEncoder batch_encoder(context);

    Plaintext plain_matrix;


    vector<uint64_t> pod_result{0ULL};


    Ciphertext encrypted_matrix;
    encryptor.encrypt(plain_matrix, encrypted_matrix);

    vector<uint64_t> pod_matrix2;

    evaluator.add_plain_inplace(encrypted_matrix, plain_matrix2);
    evaluator.square_inplace(encrypted_matrix);
    evaluator.relinearize_inplace(encrypted_matrix, relin_keys);

    Plaintext plain_result;
    decryptor.decrypt(encrypted_matrix, plain_result);
    batch_encoder.decode(plain_result, pod_result);
}


// void seal_batch_bgv_test_driver(const std::vector<int64_t>& data)
// {

//     std::array<std::vector<std::string>, 2> result{};
//     std::vector<int>                        plaintext = vecEncStringToVecInt(input, key, iv);

//     EncryptionParameters parms(scheme_type::bfv);
//     size_t               poly_modulus_degree = 8192;
//     parms.set_poly_modulus_degree(poly_modulus_degree);
//     parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

//     parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

//     SEALContext context(parms);

//     // auto qualifiers = context.first_context_data()->qualifiers();

//     KeyGenerator keygen(context);
//     SecretKey    secret_key = keygen.secret_key();
//     PublicKey    public_key;
//     keygen.create_public_key(public_key);
//     RelinKeys relin_keys;
//     keygen.create_relin_keys(relin_keys);
//     Encryptor encryptor(context, public_key);
//     Evaluator evaluator(context);
//     Decryptor decryptor(context, secret_key);

//     BatchEncoder batch_encoder(context);

//     size_t slot_count = batch_encoder.slot_count();

//     vector<uint64_t> pod_matrix(slot_count, 0ULL);

//     // Populate our matrix with our data
//     int index = 0;
//     for(const auto value : plaintext) {
//         pod_matrix[index] = static_cast<uint64_t>(value);
//         index++;
//     }

//     Plaintext plain_matrix;
//     batch_encoder.encode(pod_matrix, plain_matrix);

//     vector<uint64_t> pod_result{0ULL};
//     // batch_encoder.decode(plain_matrix, pod_result);

//     Ciphertext encrypted_matrix;
//     ;
//     encryptor.encrypt(plain_matrix, encrypted_matrix);

//     vector<uint64_t> pod_matrix2;
//     // We are using plaintext size because otherwise we are going to have a giant thing populated.
//     // We want the last actual value in the batch to be 0.
//     for(size_t i = 0; i < plaintext.size(); i++) {
//         pod_matrix2.push_back((i & size_t(0x1)) + 1);
//     }
//     Plaintext plain_matrix2;
//     batch_encoder.encode(pod_matrix2, plain_matrix2);

//     evaluator.add_plain_inplace(encrypted_matrix, plain_matrix2);
//     evaluator.square_inplace(encrypted_matrix);
//     evaluator.relinearize_inplace(encrypted_matrix, relin_keys);

//     Plaintext plain_result;
//     decryptor.decrypt(encrypted_matrix, plain_result);
//     batch_encoder.decode(plain_result, pod_result);
// }