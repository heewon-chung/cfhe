#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

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
    long security = 64;
    long m = 6361;
    long L = 20;
    long currentLength = 6;

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

    generateProblemInstance(message1, numSlots, currentLength);
    generateProblemInstance(message2, numSlots, currentLength);
    
    cout << endl << "Msg1 = ";
    Msg1 = printAndReconstructNum(message1, currentLength);
    cout << "Msg2 = ";
    Msg2 = printAndReconstructNum(message2, currentLength);
    
    ea.encrypt(ct1, publicKey, message1);
    ea.encrypt(ct2, publicKey, message2);
    
    subtract(subCt, signCt, ct1, ct2, currentLength, ea, secretKey);

    ea.decrypt(subCt, secretKey, subResult);
    ea.decrypt(signCt, secretKey, signResult);

    cout << endl;
    cout << "Subtraction Result (Plain): " << (Msg1 - Msg2) << endl;
    cout << "Subtraction Result (Encrypted): " << vector2Long(subResult, currentLength) << endl;
    cout << "Subtraction Levels Left: " << subCt.findBaseLevel() << endl;

    return 0;
}