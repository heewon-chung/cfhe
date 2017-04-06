#include "arithmetic.h"

using namespace std;
using namespace NTL;


void fullAdder(Ctxt& addCt, const Ctxt& ct1, const Ctxt& ct2, long& numLength, const EncryptedArray& ea){
    
    Ctxt productCiphertext = ct1;
    productCiphertext.multiplyBy(ct2);
    
    Ctxt sumCiphertext = ct1;
    sumCiphertext.addCtxt(ct2);
    
    numLength++;

    vector<Ctxt> tj(numLength - 1, sumCiphertext);

    for (unsigned long j = 1; j < tj.size() + 1; j++) {
        Ctxt tempCtxt = tj[j - 1];
        ea.shift(tempCtxt, -j);
        tj[j - 1] = tempCtxt;

        reverseCtxtProduct(tj[j - 1], tempCtxt, numLength - j - 1, ea);
        ea.shift(tj[j - 1], j + 1);

        // For j = i - 1
        Ctxt iMinusOneCiphertext = productCiphertext;
        Ctxt jCiphertext = productCiphertext;
        
        vector<long> mask;
        ZZX maskPoly;
        mask.resize(j - 1);
        mask.push_back(1);
        mask.resize(ea.size());
        ea.encode(maskPoly, mask);
        
        iMinusOneCiphertext.multByConstant(maskPoly);
        fillAllSlots(jCiphertext, iMinusOneCiphertext, mask, ea);

        mask.clear();
        mask.resize(j, 0);
        mask.push_back(1);
        mask.resize(ea.size());
        ea.encode(maskPoly, mask);

        tj[j - 1].addConstant(maskPoly);
        tj[j - 1].multiplyBy(jCiphertext);
    }

    for(unsigned long j = 0; j < tj.size(); j++) {
        sumCiphertext.addCtxt(tj[j]);
    }
    addCt = sumCiphertext;
}


inline void complement(Ctxt& complementCt, const Ctxt& ct, const long& numLength, const EncryptedArray& ea){
    assert(complementCt.getPubKey() == ct.getPubKey());
    assert(numLength <= ea.size());

    ZZX maskPoly;
    vector<long> mask(numLength, 1);
    mask.resize(ea.size());
    ea.encode(maskPoly, mask);

    complementCt = ct;
    complementCt.addConstant(maskPoly);
}


void subtract(Ctxt& subCt, const Ctxt& ct1, const Ctxt& ct2, long& numLength, const EncryptedArray& ea){
    assert(&ct1.getPubKey() == &ct2.getPubKey());
    assert(numLength <= ea.size());

    const FHEPubKey& publicKey = ct1.getPubKey();
    vector<long> oneVector, mask(numLength, 1);
    ZZX maskPoly;

    Ctxt oneCtxt(publicKey); Ctxt tempCtxt = ct2;
    
    // Complement of ct2
    mask.resize(ea.size());
    ea.encode(maskPoly, mask);
    tempCtxt.addConstant(maskPoly);

    oneVector.push_back(1);
    oneVector.resize(ea.size());
    ea.encrypt(oneCtxt, publicKey, oneVector);

    fullAdder(tempCtxt, tempCtxt, oneCtxt, numLength, ea);
    fullAdder(subCt, ct1, tempCtxt, numLength, ea);
    numLength--;
}