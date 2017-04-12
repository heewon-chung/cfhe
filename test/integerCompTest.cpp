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
    long L = 10;
    long currentLength = 6;
    long numPQ = 5;
    long lengthPQ = 4;

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
    
    ZZ Msg1, Msg2;
    vector<ZZX> message1, message2, compResult;
    
    Ctxt ct1(publicKey), ct2(publicKey), compCt(publicKey);

    generateProblemInstance(message1, ea.size(), currentLength);
    generateProblemInstance(message2, ea.size(), currentLength);
    
    cout << endl << "Msg1 = ";
    Msg1 = printAndReconstructNum(message1, currentLength);
    cout << "Msg2 = ";
    Msg2 = printAndReconstructNum(message2, currentLength);
    
    ea.encrypt(ct1, publicKey, message1);
    ea.encrypt(ct2, publicKey, message2);

    bool lessThan = 0;
    
    comparisonTestOverZ(compCt, ct1, ct2, lessThan, currentLength, ea);

    ea.decrypt(compCt, secretKey, compResult);

    cout << endl;
    cout << "Equal Result (Plain): ";
    if(lessThan){
        cout << (Msg1 < Msg2) << endl;
    }
    else {
        cout << (Msg1 > Msg2) << endl;
    }
    cout << "Equal Result (Encrypted): " << compResult[0] << endl;
    cout << "Equal Levels Left: " << compCt.findBaseLevel() << endl;

    return 0;
}