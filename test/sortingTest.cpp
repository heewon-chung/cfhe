#include <cstdlib>
#include <vector>

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"
#include "sorting.h"

using namespace std;
using namespace NTL;

int main(){
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 64;
    long m = 6361;
    long L = 30;
    long numPQ = 3;
    long lengthPQ = 3;
    long numData = 5;


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
    
    vector<vector<Ctxt>> sortedData;
    vector<vector<Ctxt>> data;
    

    for(unsigned long i = 0; i < numData; i++){
        vector<vector<ZZX>> message;
        generateProblemInstance(message, numSlots, numPQ, lengthPQ);

        vector<Ctxt> ct(numPQ, publicKey);
        for(unsigned long j = 0; j < numPQ; j++){
            ea.encrypt(ct[j], publicKey, message[j]);
        }
        data.push_back(ct);
    }

    directSort(sortedData, data, lengthPQ, ea);
    
    return 0;
}