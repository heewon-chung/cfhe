#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"
#include "NTL/RR.h"

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
    long L = 9;
    long numPQ = 3;
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
    long numSlots = ea.size();
    
    RR Msg1, Msg2;
    vector<vector<ZZX>> message1, message2;
    vector<long> compResult;
    vector<Ctxt> ct1(numPQ, publicKey), ct2(numPQ, publicKey);
    Ctxt compCt(publicKey);
    
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
    
    const bool lessThan = 1;
    comparisonTestOverR(compCt, ct1, ct2, lessThan, lengthPQ, ea, secretKey);

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