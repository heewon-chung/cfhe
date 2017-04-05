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
    
    long p = 2;
    long r = 1;
    long security = 64;
    long m = 6361;
    long L = 7;
    long currentLength = 6;

    FHEcontext context(m, p, r);
    buildModChain(context, L);
    ZZX F = context.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey &publicKey = secretKey;
    secretKey.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);
    long numSlots = ea.size();
    
    vector<ZZX> oneVector(numSlots, ZZX(1));
    vector<ZZX> numberOne, numberTwo, resultVector;

    generateProblemInstance(numberOne, numSlots, currentLength);
    generateProblemInstance(numberTwo, numSlots, currentLength);
    
    ZZ zzOne, zzTwo;

    zzOne = printAndReconstructNum(numberOne, currentLength);
    zzTwo = printAndReconstructNum(numberTwo, currentLength);

    Ctxt ciphertextOne(publicKey);
    Ctxt ciphertextTwo(publicKey);
    Ctxt ciphertextResult(publicKey);
    Ctxt oneCiphertext(publicKey);

    ea.encrypt(ciphertextOne, publicKey, numberOne);
    ea.encrypt(ciphertextTwo, publicKey, numberTwo);
    ea.encrypt(oneCiphertext, publicKey, oneVector);

    fullAdder(ciphertextResult, ciphertextOne, ciphertextTwo, currentLength, ea);

    ea.decrypt(ciphertextResult, secretKey, resultVector);

    cout << endl;

    cout << "Result (Plain): " << (zzOne + zzTwo) << endl;
    cout << "Result (Encrypted): " << vector2Long(resultVector, currentLength) << endl;
    cout << "Levels Left: " << ciphertextResult.findBaseLevel() << endl;

    return 0;
}