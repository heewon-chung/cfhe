#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(){
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 64;
    long m = 6361;
    long L = 8;
    long numPQ = 3;
    long lengthPQ = 5;

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
    
    vector<ZZ> Msg1, Msg2;
    vector<vector<ZZX>> message1, message2;
    vector<long> equalResult;
    vector<Ctxt> ct1(numPQ, publicKey), ct2(numPQ, publicKey);
    Ctxt equalCt(publicKey);
    
    generateProblemInstance(message1, numSlots, numPQ, lengthPQ);
    generateProblemInstance(message2, numSlots, numPQ, lengthPQ);
    
    cout << endl << "Msg1 = ";
    Msg1 = printAndReconstructNum(message1, numPQ, lengthPQ);
    cout << "Msg2 = ";
    Msg2 = printAndReconstructNum(message2, numPQ, lengthPQ);
    
    for(unsigned long i = 0; i < numPQ; i++){
        ea.encrypt(ct1[i], publicKey, message1[i]);
        ea.encrypt(ct2[i], publicKey, message2[i]);
    }
    
    equalityTestOverR(equalCt, ct1, ct2, lengthPQ, ea);

    ea.decrypt(equalCt, secretKey, equalResult);

    cout << "Equal Result (Plain): " << (Msg1 == Msg2) << endl;
    cout << "Equal Result (Encrypted): " << equalResult[0] << endl;
    cout << "Equal Levels Left: " << equalCt.findBaseLevel() << endl;

    return 0;
}