#include "comparison.h"

using namespace std;
using namespace NTL;

// Equality Test over the Integers
void equalityTestOverZ(Ctxt& equalCt, const Ctxt& ct1, const Ctxt& ct2, const long numLength, const EncryptedArray& ea){
    assert(&ct1.getPubKey() == & ct2.getPubKey());
    assert(numLength <= ea.size());

    Ctxt tempCtxt = ct1;
    ZZX onePoly;
    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);

    tempCtxt.addCtxt(ct2);
    tempCtxt.addConstant(onePoly);
    
    ctxtProduct(equalCt, tempCtxt, numLength, ea);
}


void equalityTestOverR(Ctxt& equalCt, const vector<Ctxt>& ct1, const vector<Ctxt>& ct2, long lengthPQ, const EncryptedArray& ea){
    assert(&ct1[0].getPubKey() == & ct2[0].getPubKey());
    assert(ct1.size() == ct2.size());

    long numLength = ct1.size();
    const FHEPubKey& publicKey = ct1[0].getPubKey();

    vector<Ctxt> tempCtxt = ct1, equalPQ(numLength, publicKey);
    ZZX onePoly;
    vector<long> oneVector(lengthPQ, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);

    for(int i = 0; i < numLength; i++){
        tempCtxt[i].addCtxt(ct2[i]);
        tempCtxt[i].addConstant(onePoly);

        ctxtProduct(equalPQ[i], tempCtxt[i], lengthPQ, ea);
        
        if(i == 0){
            equalCt = equalPQ[i];
        }
        else{
            equalCt.multiplyBy(equalPQ[i]);
        }
    }
}


// lessThan if lessTan = 1, otherwise greaterThan
void comparisonTestOverZ(Ctxt& compCt, const Ctxt& ct1, const Ctxt& ct2, const bool lessThan, const long numLength, const EncryptedArray& ea){
    assert(&ct1.getPubKey() == & ct2.getPubKey());
    
    Ctxt equalCt = ct1;
    ZZX onePoly, maskPoly;
    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);
    vector<long> mask(numLength, 0);
    mask[numLength - 1] = 1;
    mask.resize(ea.size());
    ea.encode(maskPoly, mask);

    equalCt.addConstant(onePoly);
    equalCt.addCtxt(ct2);

    ctxtProduct(compCt, equalCt, numLength, ea);
    ea.shift(compCt, -1);

    Ctxt tempCtxt1 = compCt, tempCtxt2 = ct1, tempCtxt3 = ct2;
    tempCtxt1.multByConstant(maskPoly);
    compCt.addCtxt(tempCtxt1);
    compCt.addConstant(maskPoly);

    if(lessThan){ // x < y
        tempCtxt2.addConstant(onePoly);
    } 
    else{ // x > y
        tempCtxt3.addConstant(onePoly);
    }
    tempCtxt2.multiplyBy(tempCtxt3);
    compCt.multiplyBy(tempCtxt2);
    tempCtxt3 = compCt;

    ctxtSum(compCt, tempCtxt3, ea.size(), ea);
}


