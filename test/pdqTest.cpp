#include <cstdlib>
#include <vector>

#include <NTL/RR.h>
#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "EncryptedData.h"
#include "EncryptedDatabase.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"
#include "queries.h"

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
    
    
    // cout << endl;
    // cout << "Add Result (Plain): " << (Msg1 + Msg2) << endl;
    // cout << "Add Result (Encrypted): " << vector2Long(addResult, currentLength) << endl;
    // cout << "Add Levels Left: " << addCt.findBaseLevel() << endl;

    return 0;
}