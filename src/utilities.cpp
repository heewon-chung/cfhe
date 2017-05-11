#include "utilities.h"

using namespace std;
using namespace NTL;


// add all messages in a ciphertext
void ctxtSum(Ctxt& addCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());

    Ctxt    tempCtxt = ct;

    addCt = ct;
    long shiftAmt = 1;
    while(shiftAmt < numLength){
        ea.shift(tempCtxt, -shiftAmt);
        tempCtxt.addCtxt(addCt);
        addCt = tempCtxt;
        shiftAmt *= 2;
    }
}


// product messages in a ciphertext
// First entry contains information
void ctxtProduct(Ctxt& mulCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());
    
    Ctxt    tempCtxt1 = ct, 
            tempCtxt2 = ct;
    long    shiftAmt = 1;

    while(shiftAmt < numLength){
        vector<long> mask(numLength - shiftAmt);
        
        mask.resize(numLength, 1);
        mask.resize(ea.size());
        ZZX maskPoly;
        ea.encode(maskPoly, mask);

        ea.shift(tempCtxt2, -shiftAmt);
        tempCtxt2.addConstant(maskPoly);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    mulCt = tempCtxt1;
}

// last entry contains information
void reverseCtxtProduct(Ctxt& mulCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea) {
    assert(numLength <= ea.size());
    
    Ctxt    tempCtxt1 = ct, 
            tempCtxt2 = ct;
    long    shiftAmt = 1;

    while (shiftAmt < numLength) {
        vector<long>    mask(shiftAmt, 1);
        ZZX             maskPoly;
        
        mask.resize(ea.size());
        ea.encode(maskPoly, mask);
        
        ea.shift(tempCtxt2, shiftAmt);
        tempCtxt2.addConstant(maskPoly);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    mulCt = tempCtxt1;
}


void fillAllSlots(Ctxt& filledCtxt, const Ctxt& orgCtxt, const vector<long>& filledPositions, const EncryptedArray& ea) {
    assert(filledPositions.size() <= ea.size());
    
    Ctxt    tempCtxt(orgCtxt.getPubKey());
    long    firstPos = 0,
            lastPos = filledPositions.size() - 1,
            totalLength = filledPositions.size();

    filledCtxt = orgCtxt;
    while (filledPositions[firstPos] != 1) {
        firstPos++;
    }
    ea.shift(filledCtxt, -firstPos);

    while (filledPositions[lastPos] != 1) {
        lastPos--;
    }

    long range = lastPos - firstPos + 1,
         currentLength = range;
    
    while (currentLength < totalLength) {
        tempCtxt = filledCtxt;
        ea.shift(tempCtxt, currentLength);
        filledCtxt += tempCtxt;
        currentLength *= 2;
    }
}