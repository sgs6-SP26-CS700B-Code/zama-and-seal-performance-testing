
#include <testTimer.hpp>
#include <zama.hpp>
#include <seal.hpp>
#include <fileProcessingHelpers.hpp>
#include <testRunnerSeal.hpp>
#include <testRunnerZama.hpp>

#include <iostream>
#include <string>

using namespace std;


int main()
{
    const string dataFile64 = "./1m-64-bit-data.data";
    cout << "Begining to read 1m lines of signed 64 bit ints into 64 bit buffer from file: " << dataFile64 << endl;
    auto signed64BitData = readFileToInt64Vector(dataFile64, 1000000);
    cout << "Finished reading 1m lines of signed 64 bit ints into 64 bit buffer from file: " << dataFile64 << endl;

    const string dataFile32 = "./1m-32-bit-data.data";
    cout << "Begining to read 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;
    auto signed32BitData = readFileToInt32Vector(dataFile32, 1000000);
    cout << "Finished reading 1m lines of signed 32 bit ints into 32 bit buffer from file: " << dataFile32 << endl;

    cout << "Begining to read 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile32 << endl;
    auto signed32BitData64BitBuffer = readFileToInt64Vector(dataFile32, 1000000);
    cout << "Finished reading 1m lines of signed 32 bit ints into 64 bit buffer from file: " << dataFile32 << endl;


    bulk_seal_tests(signed64BitData, signed32BitData, signed32BitData64BitBuffer);
    bulk_zama_tests(signed64BitData, signed32BitData, signed32BitData64BitBuffer);

    return 0;
}