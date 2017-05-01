#include "sorting.h"

/*
 *  @Input      Two Ciphertext X and Y
 *  @Output     if X <= Y, then outputs[X, Y]
 *              otherwise, outputs [Y, X]
 */
void swap(vector<Ctxt>& lessCtxt, vector<Ctxt>& greaterCtxt, const vector<Ctxt>& ctxt1, const vector<Ctxt>& ctxt2, const long lengthPQ, const EncryptedArray& ea){
    assert(&ctxt1[0].getPubKey() == &ctxt2[0].getPubKey());
    
    const bool          lessThan = 1;
    const FHEPubKey&    publicKey = ctxt1[0].getPubKey();
    vector<long>        flipVector;
    ZZX                 flipPoly;
    Ctxt                compCtxt1(publicKey), 
                        compCtxt2(publicKey), 
                        eqCtxt(publicKey);
    vector<Ctxt>        tempCtxt(ctxt2.size(), publicKey);
    
    flipVector.push_back(1);
    flipVector.resize(ea.size());
    ea.encode(flipPoly, flipVector);
    
    // compCtxt1 = LT(X, Y) = GT(Y, X)
    // compCtxt2 = LT(Y, X) = GT(X, Y) = compCtxt + 1
    comparisonTestOverR(compCtxt1, ctxt1, ctxt2, lessThan, lengthPQ, ea);
    compCtxt2 = compCtxt1;
    compCtxt2.addConstant(flipPoly);

    lessCtxt = ctxt1;
    tempCtxt = ctxt2;
    equalityTestOverR(eqCtxt, ctxt1, ctxt2, lengthPQ, ea);
    #pragma omp parallel for
    for(unsigned long i = 0; i < ctxt1.size(); i++){
        // LT(X, Y) X
        lessCtxt[i].multiplyBy(compCtxt1);
        // LT(Y, X) Y
        tempCtxt[i].multiplyBy(compCtxt2);
        // LT(X, Y) X + LT(Y, X) Y
        lessCtxt[i].addCtxt(tempCtxt[i]);
        // EQ(X, Y) X
        eqCtxt.multiplyBy(ctxt1[i]);
        // LT(X, Y) X + LT(Y, X) Y + EQ(X, Y) X
        lessCtxt[i].addCtxt(eqCtxt);
    }

    greaterCtxt = ctxt2;
    tempCtxt = ctxt1;
    #pragma omp parallel for
    for(unsigned long i = 0; i < ctxt2.size(); i++){
        // GT(X, Y) X
        greaterCtxt[i].multiplyBy(compCtxt2);
        // GT(Y, X) Y
        tempCtxt[i].multiplyBy(compCtxt1);
        // GT(X, Y) X + GT(Y, X) Y
        greaterCtxt[i].addCtxt(tempCtxt[i]);
        // EQ(X, Y) Y
        eqCtxt.multiplyBy(ctxt2[i]);
        // GT(X, Y) X + GT(Y, X) Y + EQ(X, Y) Y
        greaterCtxt[i].addCtxt(eqCtxt);
    }
    // greaterCtxt.multiplyBy(compCtxt2);
}