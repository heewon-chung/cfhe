#include "sorting.h"

using namespace std;

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


void directSort(vector<vector<Ctxt>>& sortedData, vector<vector<Ctxt>>& data, const long lengthPQ, const EncryptedArray& ea){
    
    const FHEPubKey&        publicKey = data[0][0].getPubKey();
    int                     numData = data.size(),
                            numPQ = data[0].size(),
                            numDataBits = log(numData) / log(2) + 1;
    Ctxt                    tmpCtxt(publicKey);
    vector<Ctxt>            rowAddCtxt(numData, publicKey),
                            orderCtxt(numData, publicKey);
    vector<long>            order,
                            firstSlot(ea.size());
    ZZX                     firstPoly;
    vector<vector<Ctxt>>    comparisonMatrix;

    // making all message slots 0 except first slot
    firstSlot[0] = 1;
    ea.encode(firstPoly, firstSlot);

    #pragma omp parallel for
    for(unsigned long i = 0; i < numData; i++){
        // construct a comparison matrix whose dimension is numData * numData
        vector<Ctxt> compRowMatrix(numData, publicKey);
        
        #pragma omp parallel for
        for(unsigned long j = i; j < numData; j++){
            long numBits = 0;
            if(j == 0){
                numBits = 1;
            }
            else{
                numBits = log(j) / log(2) + 1;
            }
            // (i, j) component is GT(d_i, d_j)
            comparisonTestOverR(compRowMatrix[j], data[i], data[j], 1, lengthPQ, ea);
            compRowMatrix[j].multByConstant(firstPoly);
            // add each row
            if(j == i){
                rowAddCtxt[i] = compRowMatrix[j];
            }
            else{
                fullAdder(rowAddCtxt[i], rowAddCtxt[i], compRowMatrix[j], numBits, ea);
            }
            
        }

        #pragma omp parallel for
        for(unsigned long j = 0; j < i; j++){
            long numBits = 0;
            if(j == 0){
                numBits = 1;
            }
            else{
                numBits = log(j) / log(2) + 1;
            }
            // (i, j) component = (j, i) component + 1
            compRowMatrix[j] = comparisonMatrix[j][i];
            compRowMatrix[i].addConstant(firstPoly);
            // add each row
            fullAdder(rowAddCtxt[i], rowAddCtxt[i], compRowMatrix[j], numBits, ea);
        }

        comparisonMatrix.push_back(compRowMatrix);

        // generating ciphertexts whose underlying messages are 0 ... numData - 1
        order = integer2Vector(i);
        order.resize(ea.size());
        ea.encrypt(orderCtxt[i], publicKey, order);
        order.clear();
    }

    #pragma omp parallel for
    for(unsigned long i = 0; i < numData; i++){

        #pragma omp parallel for
        for(unsigned long j = 0; j < numData; j++){
            // EQTest(rowAddCtxt[i] == i)
            equalityTestOverZ(tmpCtxt, rowAddCtxt[i], orderCtxt[j], numDataBits,ea);
            // making all zero except first slot
            tmpCtxt.multByConstant(firstPoly);
            // tmpVectorCtxt = EQTest(rowAddCtxt[i] == j) * data[i]
            vector<Ctxt> tmpVectorCtxt(numPQ, publicKey);

            #pragma omp parallel for
            for(unsigned long k = 0; k < numPQ; k++){
                tmpVectorCtxt[k] = tmpCtxt;
                tmpVectorCtxt[k].multiplyBy(data[j][k]);
                if(j != 0){
                    sortedData[i][k].addCtxt(tmpVectorCtxt[k]);
                }
            }
            if(j == 0){
                sortedData.push_back(tmpVectorCtxt);
            }
        }
    }
}