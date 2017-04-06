#include "comparison.h"

using namespace std;
using namespace NTL;

// Equality Test over the Integers
void equalityTestoverZ(Ctxt& resultCt, const Ctxt& ct1, const Ctxt& ct2, const long numLength, const EncryptedArray& ea, const FHESecKey& secretKey){
    assert(&ct1.getPubKey() == & ct2.getPubKey());
    
    const FHEPubKey& publicKey = ct1.getPubKey();
    Ctxt tempCtxt = ct1, oneCtxt(publicKey);

    vector<long> resultVector;

    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encrypt(oneCtxt, publicKey, oneVector);

    tempCtxt.addCtxt(ct2);
    tempCtxt.addCtxt(oneCtxt);
    
    reverseCtxtProduct(resultCt, tempCtxt, numLength, ea);
}


void comprisonTestoverZ(Ctxt& resultCtxt, const Ctxt& ct1, const Ctxt& ct2, const Ctxt& oneCtxt, const bool lessThan, const long numLength, const EncryptedArray &ea) {

    Ctxt equalZeroCtxt = ct1;
    equalZeroCtxt.addCtxt(oneCtxt);
    equalZeroCtxt.addCtxt(ct2);

    // requires that function uses simdShift
    ctxtProduct(resultCtxt, equalZeroCtxt, numLength, ea);

    Ctxt compCtxt = ct1;
    Ctxt tempCtxtTwo = ct2;

    if(lessThan) { // x < y
        compCtxt.addCtxt(oneCtxt);
    } 
    else { // x > y
        tempCtxtTwo.addCtxt(oneCtxt);
    }
    compCtxt.multiplyBy(tempCtxtTwo);
    ea.shift(compCtxt, 1);

    resultCtxt.multiplyBy(compCtxt);
    tempCtxtTwo = resultCtxt;
    ctxtSum(resultCtxt, tempCtxtTwo, numLength, ea);
}