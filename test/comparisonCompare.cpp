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

int main(){
	
	cout << "Homomorphic Equality Test over the Integers Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
	long d = 0;
	long c = 3;
    long L = 10;
    long m = FindM(security, L, c, p, d, 0, 0);
    long lengthPQ = 5;
    long numPQ = 3;
    long precision = 20;
    
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

    cout << endl;
    printSettings( p, r, security, m, L, numSlots );
	cout << "message precision: " << precision << endl;
    
    // Variables for Messages
    vector<vector<long>>    message1, message2;
    RR                      realMsg1, realMsg2;
    ZZ                      intMsg1, intMsg2; 
    vector<long>            bitMsg1, bitMsg2;

    // Common Variables for Encryption
    Ctxt                    intCtxt1(publicKey), intCtxt2(publicKey);
    vector<Ctxt>            realCtxt1(numPQ, publicKey), 
                            realCtxt2(numPQ, publicKey);
    Ctxt                    compCt(publicKey);

    // Timer
    TIMER start;
	TIMER end;
    
    generateProblemInstance(message1, numSlots, numPQ, lengthPQ);
    generateProblemInstance(message2, numSlots, numPQ, lengthPQ);

    NTL::RR::SetPrecision(precision);
    NTL::RR::SetOutputPrecision(precision);

    cout << endl << "original Msg1 (CF) = ";
    realMsg1 = printAndReconstructNum(message1, numPQ, lengthPQ);
    cout << "original Msg2 (CF) = ";
    realMsg2 = printAndReconstructNum(message2, numPQ, lengthPQ);
	
	cout << "original Msg1 (val): " << realMsg1 << endl;
    cout << "original Msg2 (val): " << realMsg2 << endl;

    intMsg1 = conv<ZZ>(realMsg1 * power(RR(10), precision));
    intMsg2 = conv<ZZ>(realMsg2 * power(RR(10), precision));

    long bitLength = NumBits(intMsg1);
    if(bitLength < NumBits(intMsg2)){
        bitLength = NumBits(intMsg2);
    }

    bitMsg1 = integer2Vector(intMsg1); bitMsg1.resize(numSlots);
    bitMsg2 = integer2Vector(intMsg2); bitMsg2.resize(numSlots);

    ea.encrypt(intCtxt1, publicKey, bitMsg1);
    ea.encrypt(intCtxt2, publicKey, bitMsg2);

    start = TIC;
    comparisonTestOverZ(compCt, intCtxt1, intCtxt2, 1, bitLength, ea);
	end = TOC;
    cout << "Time per integers equality test: " << get_time_us(start, end, 1) << " microsec" << std::endl;


    for(unsigned long i = 0; i < numPQ; i++){
        ea.encrypt(realCtxt1[i], publicKey, bitMsg1);
        ea.encrypt(realCtxt2[i], publicKey, bitMsg2);
    }

    compCt.clear();

    start = TIC;
    comparisonTestOverR(compCt, realCtxt1, realCtxt2, 1, lengthPQ, ea);
	end = TOC;
	cout << "Time per reals Equality test: " << get_time_us(start, end, 1) << " microsec" << endl;

}