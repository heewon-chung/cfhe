#include "utilities.h"

using namespace std;
using namespace NTL;


// add all messages in a ciphertext
void ctxtSum(Ctxt& addCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());

    Ctxt tempCtxt1 = ct;
    Ctxt tempCtxt2 = ct;

    long shiftAmt = 1;
    while(shiftAmt < numLength){
        ea.shift(tempCtxt2, -shiftAmt);
        tempCtxt2.addCtxt(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    addCt = tempCtxt1;
}


// product messages in a ciphertext
// First entry contains information
void ctxtProduct(Ctxt& mulCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());

    Ctxt tempCtxt1 = ct, tempCtxt2 = ct;

    long shiftAmt = 1;
    while(shiftAmt < numLength){
        ea.shift(tempCtxt2, -shiftAmt);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    mulCt = tempCtxt1;
}

// last entry contains information
void reverseCtxtProduct(Ctxt& prodCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea) {
    assert(numLength <= ea.size());

    Ctxt tempCtxt1 = ct, tempCtxt2 = ct;

    long shiftAmt = 1;
    while (shiftAmt < numLength) {
        ea.shift(tempCtxt2, shiftAmt);

        vector<long> mask(shiftAmt, 1);
        mask.resize(ea.size());
        ZZX maskPoly;
        ea.encode(maskPoly, mask);
        tempCtxt2.addConstant(maskPoly);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;

        shiftAmt *= 2;
    }

    prodCt = tempCtxt1;
}


void fillAllSlots(Ctxt &ciphertextFilled, const Ctxt &ciphertext, const vector<long> &filledPositions, const EncryptedArray& ea) {
    assert(filledPositions.size() <= ea.size());
    ciphertextFilled = ciphertext;
    Ctxt tempCtxt(ciphertext.getPubKey());

    long firstPos = 0;
    while (filledPositions[firstPos] != 1) {
        firstPos++;
    }
    ea.shift(ciphertextFilled, -firstPos);

    long lastPos = filledPositions.size() - 1;
    while (filledPositions[lastPos] != 1) {
        lastPos--;
    }
    long range = lastPos - firstPos + 1;
    long currentLength = range;
    long totalLength = filledPositions.size();
    while (currentLength < totalLength) {
        tempCtxt = ciphertextFilled;
        ea.shift(tempCtxt, currentLength);
        ciphertextFilled += tempCtxt;
        currentLength *= 2;
    }
}