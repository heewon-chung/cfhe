//
// Created by ben on 20/12/2016.
//
#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "generalTools.h"
#include "comparison.h"
#include "arithmetic.h"

int main() {

    long p = 2;
    long r = 1;
    long m = 6361;
    long L = 7;
    long currentLength = 3;
    long numLength = 10;

    FHEcontext context(m, p, r);
    buildModChain(context, L);
    ZZX F = context.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey &publicKey = secretKey;
    secretKey.GenSecKey(64);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);

    long numSlots = ea.size();
    long plaintextDegree = ea.getDegree();

    vector<ZZX> oneVector(numSlots, ZZX(1));
    vector<ZZX> numberOne, numberTwo, resultVector;

    generateProblemInstance(numberOne, numSlots, currentLength);
    generateProblemInstance(numberTwo, numSlots, currentLength);

    Ctxt ciphertextOne(publicKey);
    Ctxt ciphertextTwo(publicKey);
    Ctxt ciphertextResult(publicKey);
    Ctxt tempCiphertext(publicKey);
    Ctxt tempCiphertextOne(publicKey);
    Ctxt oneCiphertext(publicKey);

    ea.encrypt(ciphertextOne, publicKey, numberOne);
    ea.encrypt(ciphertextTwo, publicKey, numberTwo);
    ea.encrypt(oneCiphertext, publicKey, oneVector);

    tempCiphertext = oneCiphertext;
    tempCiphertextOne = oneCiphertext;
    ciphertextResult = oneCiphertext;

    bool lessThan = 1;

//    ordTest(ciphertextResult, ciphertextOne, ciphertextTwo, oneCiphertext, lessThan, numLength, ea);
    Ctxt equalZeroCtxt = ciphertextOne;
    equalZeroCtxt.addCtxt(oneCiphertext);
    equalZeroCtxt.addCtxt(ciphertextTwo);

    // requires that function uses simdShift
//    cout << "Level before Product: " << equalZeroCtxt.findBaseLevel() << endl;
    ctxtProduct(ciphertextResult, equalZeroCtxt, numLength, ea);
//    cout << "Level after Product: " << ciphertextResult.findBaseLevel() << endl;
//    simdShift(equalZeroCtxt, resultCtxt, -1, numLength, ea);
    ea.shift(ciphertextResult, -1);
    Ctxt prefixedCtxt = ciphertextResult;

    vector<long> correctionVec(numLength, 0);
    correctionVec[numLength-1] = 1;
    correctionVec.resize(numSlots);
    ZZX correctionPoly;
    ea.encode(correctionPoly, correctionVec);

    Ctxt tempCtxtX = ciphertextResult;
    tempCtxtX.multByConstant(correctionPoly);
    ciphertextResult.addCtxt(tempCtxtX);
    ciphertextResult.addConstant(correctionPoly);

    Ctxt fixedCtxt = ciphertextResult;

    Ctxt compCtxt = ciphertextOne;
    Ctxt tempCtxtTwo = ciphertextTwo;

    if(lessThan) { // x < y
        compCtxt.addCtxt(oneCiphertext);
    } else { // x > y
        tempCtxtTwo.addCtxt(oneCiphertext);
    }
    compCtxt.multiplyBy(tempCtxtTwo);
//    ea.shift(compCtxt, 1);

    ciphertextResult.multiplyBy(compCtxt);
    tempCtxtTwo = ciphertextResult;
//    cout << "Level before Sum: " << ciphertextResult.findBaseLevel() << endl;
    ctxtSum(ciphertextResult, tempCtxtTwo, numLength, ea);
//    cout << "Level after Sum: " << ciphertextResult.findBaseLevel() << endl;
    endTime = chrono::high_resolution_clock::now();
    timeTaken = endTime - startTime;

    ZZ zzOne, zzTwo;

    zzOne = printAndReconstructNum(numberOne, currentLength);
    zzTwo = printAndReconstructNum(numberTwo, currentLength);
    cout << endl << "Number One: " << zzOne << ", Number Two: " << zzTwo << endl;


    ea.decrypt(ciphertextResult, secretKey, resultVector);
//    cout << endl << "Final Result: ";
//    for(unsigned long i = 0; i < currentLength+2; i++) {
//        cout << resultVector[i] << ", ";
//    }

//    cout << vector2Long(resultVector, numLength) << endl;

    cout << endl << "Result (Plain): ";
    if(lessThan){
        cout << (zzOne < zzTwo) << endl;
    }
    else {
        cout << (zzOne > zzTwo) << endl;
    }
    cout << "Result (Encrypted): " << resultVector[0] << endl;
    cout << "Levels Left: " << ciphertextResult.findBaseLevel() << endl;
    cout << "Time Taken: " << timeTaken.count() << endl;

    return 0;
}