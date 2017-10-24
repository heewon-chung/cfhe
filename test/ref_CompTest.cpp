#include <cstdlib>
#include <vector>

#include <NTL/ZZ.h>
#include <NTL/RR.h>

#include "FHE.h"
#include "EncryptedArray.h"

#include "generateMsg.h"
#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(int argc, char* argv[]){
    
    if(argc != 5){
        cerr << "\nplease enter three parameter for test\n";
        cerr << "such as numPQ, lengthPQ, L and precision...\n\n";

        return -1;
    }

	cout << "\nHomomorphic Equality Test over the Integers Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
	long d = 0;
	long c = 3;
    long L = 0;
    long lengthPQ = 0;
    long numPQ = 0;
    long precision = 0;
    
    if(argc > 1){
        numPQ = atoi(argv[1]);
		lengthPQ = atoi(argv[2]);
        L = atoi(argv[3]);
        precision = atoi(argv[4]);
    }
    
    long m = FindM(security, L, c, p, d, 0, 0);
    FHEcontext context(m, p, r);
    buildModChain(context, L);
    
    ZZX F = context.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;
    secretKey.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);
    long numSlots = ea.size();
    
    // Variables for Messages
    vector<vector<long>>    message1, message2;
    RR                      realMsg1, realMsg2;
    ZZ                      intMsg1, intMsg2; 
    vector<long>            bitMsg1, bitMsg2, 
                            compResult;

    // Variables for Encryption
    Ctxt                    ct1(publicKey), ct2(publicKey);
    Ctxt                    compCt(publicKey);

    // Timer
    TIMER start;
	TIMER end;
    
    generateProblemInstance(message1, numSlots, numPQ, lengthPQ);
    generateProblemInstance(message2, numSlots, numPQ, lengthPQ);

    NTL::RR::SetPrecision(precision);
    NTL::RR::SetOutputPrecision(precision);

    cout << endl << "original Msg1 (CF): ";
    realMsg1 = printAndReconstructNum(message1, numPQ, lengthPQ);
    cout << "original Msg2 (CF): ";
    realMsg2 = printAndReconstructNum(message2, numPQ, lengthPQ);

    intMsg1 = conv<ZZ>(realMsg1 * power(RR(10), precision));
    intMsg2 = conv<ZZ>(realMsg2 * power(RR(10), precision));

    cout << "Input Msg1 (val): " << intMsg1 << endl;
    cout << "Input Msg2 (val): " << intMsg2 << endl;

    long bitLength = NumBits(intMsg1);
    if(bitLength < NumBits(intMsg2)){
        bitLength = NumBits(intMsg2);
    }
    cout << endl;
    printSettings(L, numPQ, lengthPQ, precision, bitLength);

    bitMsg1 = integer2Vector(intMsg1, bitLength); bitMsg1.resize(numSlots);
    bitMsg2 = integer2Vector(intMsg2, bitLength); bitMsg2.resize(numSlots);

    ea.encrypt(ct1, publicKey, bitMsg1);
    ea.encrypt(ct2, publicKey, bitMsg2);

    const bool lessThan = 1;
	string str;
	if (lessThan == 1){
        str = "Less Than";
    }
	else{
        str = "Greater Than";
    }
    
    start = TIC;
    comparisonTestOverZ(compCt, ct1, ct2, 1, bitLength, ea);
	end = TOC;
    cout << "\nTime per Reals Comparison (" << str << ") Evaluation Time: " << get_time_us(start, end, 1) << " microsec" << endl;
    cout << "Comparison Levels Left: " << compCt.findBaseLevel() << endl;

    return 0;
}