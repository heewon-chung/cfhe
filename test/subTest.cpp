#include <cstdlib>
#include <vector>

//#include "../../../Library/HElib-master/src/EncryptedArray.h"
//#include "../../../Library/HElib-master/src/Ctxt.h"

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generalTools.h"
#include "utilities.h"
#include "arithmetic.h"

using namespace std;
using namespace NTL;

int main(){
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
    long L = 20;
    long bitSize = 6;
    long c = 3;
    long m = FindM(security, L, c, p, 0, 0, 0);

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
    
    ZZ Msg1, Msg2;
    vector<ZZX> message1, message2, subResult;
    vector<long> signResult;
    
    Ctxt ct1(publicKey), ct2(publicKey), subCt(publicKey), signCt(publicKey);

    generateProblemInstance(message1, numSlots, bitSize);
    generateProblemInstance(message2, numSlots, bitSize);
    
    cout << endl << "Msg1 = ";
    Msg1 = printAndReconstructNum(message1, bitSize);
    cout << "Msg2 = ";
    Msg2 = printAndReconstructNum(message2, bitSize);
    
    ea.encrypt(ct1, publicKey, message1);
    ea.encrypt(ct2, publicKey, message2);
    
    TIMER start;
	TIMER end;
	start = TIC;
    subtract(subCt, signCt, ct1, ct2, bitSize, ea);
    end = TOC;

    ea.decrypt(subCt, secretKey, subResult);
    ea.decrypt(signCt, secretKey, signResult);

    cout << endl;
    cout << "Subtraction Result (Plain): " << (Msg1 - Msg2) << endl;
    cout << "Subtraction Result (Encrypted): " << vector2Long(subResult, bitSize) << endl;
    cout << "Subtraction Levels Left: " << subCt.findBaseLevel() << endl;
    cout << "Evaluation time for Binary Subtraction: " << get_time_us(start, end, 1) / 1000000 << " sec" << endl;
    return 0;
}