#include "arithmetic.h"

using namespace std;
using namespace NTL;


void fullAdder(Ctxt& addCt, const Ctxt& ctxt1, const Ctxt& ctxt2, long& numLength, const EncryptedArray& ea){
    assert(&ctxt1.getPubKey() == &ctxt2.getPubKey());
    numLength++;

    Ctxt prodCtxt = ctxt1, sumCtxt = ctxt1;
    vector<Ctxt> tj(numLength - 1, sumCtxt);

    prodCtxt.multiplyBy(ctxt2);
    sumCtxt.addCtxt(ctxt2);

    for (unsigned long j = 1; j < tj.size() + 1; j++) {
        Ctxt tempCtxt = tj[j - 1];
        ea.shift(tempCtxt, -j);
        tj[j - 1] = tempCtxt;

        reverseCtxtProduct(tj[j - 1], tempCtxt, numLength - j - 1, ea);
        ea.shift(tj[j - 1], j + 1);

        // for j = i - 1
        Ctxt iMinusOneCiphertext = prodCtxt;
        Ctxt jCtxt = prodCtxt;
        
        vector<long> mask;
        ZZX maskPoly;
        mask.resize(j - 1);
        mask.push_back(1);
        mask.resize(ea.size());
        ea.encode(maskPoly, mask);
        
        iMinusOneCiphertext.multByConstant(maskPoly);
        fillAllSlots(jCtxt, iMinusOneCiphertext, mask, ea);

        mask.clear();
        mask.resize(j, 0);      mask.push_back(1);      mask.resize(ea.size());
        ea.encode(maskPoly, mask);

        tj[j - 1].addConstant(maskPoly);
        tj[j - 1].multiplyBy(jCtxt);
    }

    for(unsigned long j = 0; j < tj.size(); j++) {
        sumCtxt.addCtxt(tj[j]);
    }
    addCt = sumCtxt;
}


inline void complement(Ctxt& complementCtxt, const Ctxt& ct, const long& numLength, const EncryptedArray& ea){
    assert(&complementCtxt.getPubKey() == &ct.getPubKey());
    assert(numLength <= ea.size());

    ZZX maskPoly;
    vector<long> mask(numLength, 1);
    mask.resize(ea.size());
    ea.encode(maskPoly, mask);

    complementCtxt = ct;
    complementCtxt.addConstant(maskPoly);
}


inline void twoComplement(Ctxt& complementCtxt, const Ctxt& ct, long& numLength, const EncryptedArray& ea){
    assert(&complementCtxt.getPubKey() == &ct.getPubKey());
    assert(numLength <= ea.size());

    const FHEPubKey& publicKey = ct.getPubKey();
    // complement of ct
    ZZX maskPoly;
    vector<long> mask(numLength, 1), one;
    Ctxt oneCtxt(publicKey);
    mask.resize(ea.size());     ea.encode(maskPoly, mask);
    one.push_back(1);           one.resize(ea.size());

    complementCtxt = ct;
    complementCtxt.addConstant(maskPoly);

    ea.encrypt(oneCtxt, publicKey, one);
    // add 1 to the complement of ct
    fullAdder(complementCtxt, complementCtxt, oneCtxt, numLength, ea);
}


void subtract(Ctxt& subCtxt, Ctxt& signCtxt, const Ctxt& ctxt1, const Ctxt& ctxt2, const long& numLength, const EncryptedArray& ea, const FHESecKey& secretKey){
    assert(&ctxt1.getPubKey() == &ctxt2.getPubKey());
    assert(numLength <= ea.size());
    
    long bitSize = numLength;
    const FHEPubKey& publicKey = ctxt1.getPubKey();
    ZZX flipPoly, signPoly;
    Ctxt complementCtxt(publicKey), oneCtxt(publicKey);
    vector<long> signVector(bitSize), flipVector(ea.size(), 1);
    signVector.push_back(1);    signVector.resize(ea.size());   ea.encode(signPoly, signVector);    
    ea.encode(flipPoly, flipVector);

    // 2's complement of ctxt2
    twoComplement(complementCtxt, ctxt2, bitSize, ea);
    // ctxt1 + 2's complement of ctxt2
    fullAdder(subCtxt, ctxt1, complementCtxt, bitSize, ea);
    // MSB contains an information about the sign
    fillAllSlots(signCtxt, subCtxt, signVector, ea);
    // discard the MSB
    subCtxt.addConstant(signPoly);

    Ctxt tempCtxt = subCtxt;
    // For MSB = 1 (ctxt1 > ctxt2),
    // MSB * subCtxt
    subCtxt.multiplyBy(signCtxt);
    // For MSB = 0 (ctxt1 < ctxt2),
    Ctxt flipSignCtxt = signCtxt;
    flipSignCtxt.addConstant(flipPoly);
    twoComplement(tempCtxt, tempCtxt, bitSize, ea);
    // (MSB + 1) * 2's complement of subCtxt
    tempCtxt.multiplyBy(flipSignCtxt);
    // MSB * subCtxt + (MSB + 1) * 2's complement of subCtxt
    subCtxt.addCtxt(tempCtxt);
}