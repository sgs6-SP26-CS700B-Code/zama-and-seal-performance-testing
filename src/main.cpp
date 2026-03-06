
#include <testTimer.hpp>
// #include <zama.hpp>
// #include <seal.hpp>
#include <fileProcessingHelpers.hpp>
// #include <testRunnerSeal.hpp>
// #include <testRunnerZama.hpp>
#include <zama-vs-seal.hpp>

#include <iostream>
#include <string>

using namespace std;


int main()
{
    // const string dataFile64 = "./1m-64-bit-data.data";
    // cout << "Begining to read 1m lines of signed 64 bit ints into 64 bit buffer from file: " << dataFile64 << endl;
    // auto signed64BitData = readFileToInt64Vector(dataFile64, 1000000);
    // cout << "Finished reading 1m lines of signed 64 bit ints into 64 bit buffer from file: " << dataFile64 << endl;

    // const string dataFile32 = "./1m-32-bit-data.data";
    // cout << "Begining to read 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;
    // auto signed32BitData = readFileToInt32Vector(dataFile32, 1000000);
    // cout << "Finished reading 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;

    // cout << "Begining to read 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile32 << endl;
    // auto signed32BitData64BitBuffer = readFileToInt64Vector(dataFile32, 1000000);
    // cout << "Finished reading 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile32 << endl;


    const string dataFile16 = "./1m-16-bit-data.data";
    cout << "Begining to read 1m lines of signed 16 bit ints into 16 bit buffer from file: " << dataFile16 << endl;
    auto u16BitData = readFileToUInt16Vector(dataFile16, 1000000);
    cout << "Finished reading 1m lines of signed 16 bit ints into 16 bit buffer from file: " << dataFile16 << endl;


    // const string dataFile32 = "./1m-32-bit-data.data";
    // cout << "Begining to read 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;
    // auto unsign32BitData = readFileToUInt32Vector(dataFile32, 1000000);
    // cout << "Finished reading 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;


    // cout << "\nPerforming seal 16384 batch size on 800 value dataset:\n\n";
    // std::cout << std::flush;
    // seal_test_16384_batch_size(getFirstNValuesU32FromU16(u16BitData, 800));

    // cout << "\nPerforming zama serial compute on 800 value dataset:\n\n";
    // std::cout << std::flush;
    // zama_test_serial_compute(getFirstNValuesU32FromU16(u16BitData, 800));


    cout << "\nPerforming seal 16384 batch size on 1 mil value dataset:\n\n";
    cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    std::cout << std::flush;
    auto data = getFirstNValuesU32FromU16(u16BitData, 1000000);

    for (int i = 1; i<=10; i++) {
        cout << "SEAL TEST # " << i << endl;
        seal_test_16384_batch_size(data);
    }

    // cout << "\nPerforming zama serial compute on 1 mil value dataset:\n\n";
    // std::cout << std::flush;
    // zama_test_serial_compute(getFirstNValuesU32FromU16(u16BitData, 1000000));



    // currently does 819*2 because I forgot I can just loop to make it not the same value. Meh, actual
    // arithmetic is across 819.
    // testSeal_32_oneHot(819 * 2, "sealOneHot", unsign32BitData);

    // testZama_32_oneHot(819, "zamaOneHot", unsign32BitData);

    // bulk_seal_tests(signed64BitData, signed32BitData, signed32BitData64BitBuffer);
    // bulk_zama_tests(signed64BitData, signed32BitData, signed32BitData64BitBuffer);

    return 0;
}