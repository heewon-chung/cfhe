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
    long L = 6;
    long currentLength = 10;

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
    vector<ZZX> message1, message2, equalResult;
    
    Ctxt ct1(publicKey), ct2(publicKey), equalCt(publicKey);

    generateProblemInstance(message1, numSlots, currentLength);
    generateProblemInstance(message2, numSlots, currentLength);
    
    cout << endl << "Msg1 = ";
    Msg1 = printAndReconstructNum(message1, currentLength);
    cout << "Msg2 = ";
    Msg2 = printAndReconstructNum(message2, currentLength);
    
    ea.encrypt(ct1, publicKey, message1);
    ea.encrypt(ct2, publicKey, message2);
    
    equalityTestoverZ(equalCt, ct1, ct2, currentLength, ea);

    ea.decrypt(equalCt, secretKey, equalResult);

    cout << endl;
    cout << "Equal Result (Plain): " << (Msg1 == Msg2) << endl;
    cout << "Equal Result (Encrypted): " << equalResult[0] << endl;
    cout << "Equal Levels Left: " << equalCt.findBaseLevel() << endl;

    return 0;
}