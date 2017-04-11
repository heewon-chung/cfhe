#include "comparison.h"

using namespace std;
using namespace NTL;

// Equality Test over the Integers
void equalityTestoverZ(Ctxt& equalCt, const Ctxt& ct1, const Ctxt& ct2, const long numLength, const EncryptedArray& ea){
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


void equalityTestoverR(Ctxt& equalCt, const Ctxt& ct1, const Ctxt& ct2, const long numPQ, long lengthPQ, const EncryptedArray& ea, const FHESecKey& secretKey){
    assert(&ct1.getPubKey() == & ct2.getPubKey());
    assert(numPQ * lengthPQ <= ea.size());
    
    Ctxt tempCtxt = ct1;
    ZZX onePoly;
    vector<long> oneVector(numPQ * lengthPQ, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);

    vector<long> result;

    tempCtxt.addCtxt(ct2);

    ea.decrypt(tempCtxt, secretKey, result);
    cout << "result1 = " << result << endl;
    result.clear();

    tempCtxt.addConstant(onePoly);
    ea.decrypt(tempCtxt, secretKey, result);
    cout << "result2 = " << result << endl;
    result.clear();
    
    ctxtProduct(equalCt, tempCtxt, numPQ * lengthPQ, ea);
}


// lessThan if lessTan = 1, otherwise greaterThan
void comparisonTestoverZ(Ctxt& compCt, const Ctxt& ct1, const Ctxt& ct2, const bool lessThan, const long numLength, const EncryptedArray& ea){
    assert(&ct1.getPubKey() == & ct2.getPubKey());
    Ctxt equalCt = ct1;
    ZZX onePoly, correctionPoly;
    vector<long> oneVector(numLength, 1);
    oneVector.resize(ea.size());
    ea.encode(onePoly, oneVector);
    vector<long> correctionVec(numLength, 0);
    correctionVec[numLength - 1] = 1;
    correctionVec.resize(ea.size());
    ea.encode(correctionPoly, correctionVec);

    equalCt.addConstant(onePoly);
    equalCt.addCtxt(ct2);

    ctxtProduct(compCt, equalCt, numLength, ea);
    ea.shift(compCt, -1);

    Ctxt tempCtxt1 = compCt, tempCtxt2 = ct1, tempCtxt3 = ct2;
    tempCtxt1.multByConstant(correctionPoly);
    compCt.addCtxt(tempCtxt1);
    compCt.addConstant(correctionPoly);

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