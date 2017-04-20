#include "comparison.h"

using namespace std;
using namespace NTL;

// Equality Test over the Integers
void equalityTestOverZ(Ctxt& equalCt, const Ctxt& ctxt1, const Ctxt& ctxt2, const long numLength, const EncryptedArray& ea){
    assert(&ctxt1.getPubKey() == & ctxt2.getPubKey());
    assert(numLength <= ea.size());

    Ctxt tempCtxt = ctxt1;
    ZZX onePoly;
    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);

    tempCtxt.addCtxt(ctxt2);
    tempCtxt.addConstant(onePoly);
    
    ctxtProduct(equalCt, tempCtxt, numLength, ea);
}


void equalityTestOverR(Ctxt& equalCt, const vector<Ctxt>& ctxt1, const vector<Ctxt>& ctxt2, long lengthPQ, const EncryptedArray& ea){
    assert(&ctxt1[0].getPubKey() == & ctxt2[0].getPubKey());
    assert(ctxt1.size() == ctxt2.size());

    long numPQ = ctxt1.size();
    const FHEPubKey& publicKey = ctxt1[0].getPubKey();

    vector<Ctxt> equalPQ(numPQ, publicKey);
    #pragma omp parallel for
    for(unsigned long i = 0; i < numPQ; i++){
        equalityTestOverZ(equalPQ[i], ctxt1[i], ctxt2[i], lengthPQ, ea);
        if(i == 0){
            equalCt = equalPQ[i];
        }
        else{
            equalCt.multiplyBy(equalPQ[i]);
        }
    }
    // ZZX onePoly;
    // vector<long> oneVector(lengthPQ, 1);
    // oneVector.resize(ea.size());
    // ea.encode(onePoly, oneVector);

    // for(int i = 0; i < numPQ; i++){
    //     tempCtxt[i].addCtxt(ctxt2[i]);
    //     tempCtxt[i].addConstant(onePoly);

    //     ctxtProduct(equalPQ[i], tempCtxt[i], lengthPQ, ea);
        
    //     if(i == 0){
    //         equalCt = equalPQ[i];
    //     }
    //     else{
    //         equalCt.multiplyBy(equalPQ[i]);
    //     }
    // }
}


// lessThan if lessTan = 1, otherwise greaterThan
void comparisonTestOverZ(Ctxt& compCt, const Ctxt& ctxt1, const Ctxt& ctxt2, const bool lessThan, const long numLength, const EncryptedArray& ea){
    assert(&ctxt1.getPubKey() == & ctxt2.getPubKey());
    
    Ctxt equalCt = ctxt1;
    ZZX onePoly, maskPoly;
    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);
    equalCt.addConstant(onePoly);
    equalCt.addCtxt(ctxt2);

    ctxtProduct(compCt, equalCt, numLength, ea);
    ea.shift(compCt, -1);

    Ctxt tempCtxt1 = compCt, tempCtxt2 = ctxt1, tempCtxt3 = ctxt2;
    
    vector<long> mask(ea.size());
    mask[numLength - 1] = 1;
    ea.encode(maskPoly, mask);
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


void comparisonTestOverR(Ctxt& compCt, const vector<Ctxt>& ctxt1, const vector<Ctxt>& ctxt2, const bool lessThan, const long lengthPQ, const EncryptedArray& ea){
    assert(&ctxt1[0].getPubKey() == & ctxt2[0].getPubKey());
    
    const bool greaterThan = 1 - lessThan;
    long numPQ = ctxt1.size();
    const FHEPubKey& publicKey = ctxt1[0].getPubKey();
    Ctxt equalCt(publicKey), prodCt(publicKey);
    vector<Ctxt> equalPQ(numPQ, publicKey), cmpCtxt(numPQ, publicKey);
    vector<long> extractFirstVector;
    extractFirstVector.push_back(1);
    extractFirstVector.resize(ea.size());
    ZZX extractFirstPoly;
    ea.encode(extractFirstPoly, extractFirstVector);

#pragma omp parallel for
    for(unsigned long i = 0; i < numPQ; i++){
        equalityTestOverZ(equalPQ[i], ctxt1[i], ctxt2[i], lengthPQ, ea);
        equalPQ[i].multByConstant(extractFirstPoly);
        ea.shift(equalPQ[i], i);
        equalCt.addCtxt(equalPQ[i]);
    }
    reverseCtxtProduct(prodCt, equalCt, numPQ, ea);
    Ctxt tempCtxt1 = prodCt, tempCtxt2 = prodCt;
    ea.shift(tempCtxt2, -1);
#pragma omp parallel for
    for(unsigned long i = 0; i < numPQ / 2; i++){
        if(lessThan){
            comparisonTestOverZ(cmpCtxt[2 * i], ctxt1[2 * i], ctxt2[2 * i], lessThan, lengthPQ, ea);
            if(i == 0){
                compCt.addCtxt(cmpCtxt[0]);
            }
            else{
                ea.shift(tempCtxt1, -2);
                cmpCtxt[2 * i].multiplyBy(tempCtxt1);
                compCt.addCtxt(cmpCtxt[2 * i]);
            }
            comparisonTestOverZ(cmpCtxt[2 * i + 1], ctxt1[2 * i + 1], ctxt2[2 * i + 1], greaterThan, lengthPQ, ea);
            ea.shift(tempCtxt2, -2);
            cmpCtxt[2 * i + 1].multiplyBy(tempCtxt2);
            compCt.addCtxt(cmpCtxt[2 * i + 1]);
        }
        else{
            comparisonTestOverZ(cmpCtxt[2 * i], ctxt1[2 * i], ctxt2[2 * i], greaterThan, lengthPQ, ea);
            if(i == 0){
                compCt.addCtxt(cmpCtxt[0]);
            }
            else{
                ea.shift(tempCtxt1, -2);
                cmpCtxt[2 * i].multiplyBy(tempCtxt1);
                compCt.addCtxt(cmpCtxt[2 * i]);
            }
            comparisonTestOverZ(cmpCtxt[2 * i + 1], ctxt1[2 * i + 1], ctxt2[2 * i + 1], lessThan, lengthPQ, ea);
            ea.shift(tempCtxt2, -2);
            cmpCtxt[2 * i + 1].multiplyBy(tempCtxt2);
            compCt.addCtxt(cmpCtxt[2 * i + 1]);
        }   
    }
}