#include "arithmetic.h"

using namespace std;
using namespace NTL;


void fullAdder(Ctxt& addCt, const Ctxt& ctxt1, const Ctxt& ctxt2, long& bitSize, const EncryptedArray& ea){
    assert(&ctxt1.getPubKey() == &ctxt2.getPubKey());
    bitSize++;

    Ctxt            prodCtxt = ctxt1, 
                    sumCtxt = ctxt1;
    vector<Ctxt>    carry(bitSize, sumCtxt);

    prodCtxt.multiplyBy(ctxt2);
    sumCtxt.addCtxt(ctxt2);

    #pragma omp parallel for
    for (unsigned long j = 1; j < carry.size() + 1; j++) {
        // Ctxt tempCtxt = carry[j - 1];

        // ea.shift(tempCtxt, -j);
        // carry[j - 1] = tempCtxt;
        // reverseCtxtProduct(carry[j - 1], tempCtxt, bitSize - j - 1, ea);
        ea.shift(carry[j - 1], -j);
        reverseCtxtProduct(carry[j - 1], carry[j - 1], bitSize - j - 1, ea);
        ea.shift(carry[j - 1], j + 1);

        // for j = i - 1
        Ctxt            iMinusOneCiphertext = prodCtxt,
                        jCtxt = prodCtxt;
        vector<long>    mask;
        ZZX             maskPoly;

        mask.resize(j - 1);     mask.push_back(1);      mask.resize(ea.size());
        ea.encode(maskPoly, mask);
        
        iMinusOneCiphertext.multByConstant(maskPoly);
        fillAllSlots(jCtxt, iMinusOneCiphertext, mask, ea);

        mask.clear();           mask.resize(j, 0);      mask.push_back(1);      mask.resize(ea.size());
        ea.encode(maskPoly, mask);

        carry[j - 1].addConstant(maskPoly);
        carry[j - 1].multiplyBy(jCtxt);
    }

    #pragma omp parallel for
    for(unsigned long j = 0; j < carry.size(); j++) {
        sumCtxt.addCtxt(carry[j]);
    }
    addCt = sumCtxt;
}


inline void complement(Ctxt& complementCtxt, const Ctxt& ct, const EncryptedArray& ea){
    assert(&complementCtxt.getPubKey() == &ct.getPubKey());

    ZZX             maskPoly;
    vector<long>    mask(ea.size(), 1);

    ea.encode(maskPoly, mask);

    complementCtxt = ct;
    complementCtxt.addConstant(maskPoly);
}


inline void complement(Ctxt& complementCtxt, const Ctxt& ct, const long& numLength, const EncryptedArray& ea){
    assert(&complementCtxt.getPubKey() == &ct.getPubKey());
    assert(numLength <= ea.size());

    ZZX             maskPoly;
    vector<long>    mask(numLength, 1);
    
    mask.resize(ea.size());     
    ea.encode(maskPoly, mask);

    complementCtxt = ct;
    complementCtxt.addConstant(maskPoly);
}


inline void twoComplement(Ctxt& complementCtxt, const Ctxt& ct, long& numLength, const EncryptedArray& ea){
    assert(&complementCtxt.getPubKey() == &ct.getPubKey());
    assert(numLength <= ea.size());

    const FHEPubKey&    publicKey = ct.getPubKey();
    ZZX                 maskPoly;
    vector<long>        mask(numLength, 1), 
                        one;
    Ctxt                oneCtxt(publicKey);

    mask.resize(ea.size());     
    ea.encode(maskPoly, mask);
    one.push_back(1);           one.resize(ea.size());

    complementCtxt = ct;
    complementCtxt.addConstant(maskPoly);

    ea.encrypt(oneCtxt, publicKey, one);
    // add 1 to the complement of ct
    fullAdder(complementCtxt, complementCtxt, oneCtxt, numLength, ea);
}


void subtract(Ctxt& subCtxt, Ctxt& signCtxt, const Ctxt& ctxt1, const Ctxt& ctxt2, const long& numLength, const EncryptedArray& ea){
    assert(&ctxt1.getPubKey() == &ctxt2.getPubKey());
    assert(numLength <= ea.size());
    
    long                bitSize = numLength;
    const FHEPubKey&    publicKey = ctxt1.getPubKey();
    ZZX                 flipPoly, signPoly;
    Ctxt                complementCtxt(publicKey), 
                        oneCtxt(publicKey);
    vector<long>        signVector(bitSize), 
                        flipVector(ea.size(), 1);
    
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


void restoringDivision(Ctxt& quoCtxt, Ctxt& remCtxt, const Ctxt& numCtxt, const Ctxt& denCtxt, const long& numLength, const EncryptedArray& ea, const FHESecKey& secretKey){
    assert(&numCtxt.getPubKey() == &denCtxt.getPubKey());
    assert(numLength <= ea.size());

    const FHEPubKey&    publicKey = numCtxt.getPubKey();
    Ctxt                signCtxt(publicKey), 
                        tempCtxt(publicKey), 
                        tempDenCtxt = denCtxt;
    vector<long> result;

    // P = numCtxt;
    remCtxt = numCtxt;
    // D << numLength
    ea.shift(tempDenCtxt, numLength / 2);
    
    for(int i = numLength / 2 - 1; i >= 0; i--){
        vector<long>    signVector(ea.size());
        ZZX             signPoly;
        
        signVector[i] = 1;
        ea.encode(signPoly, signVector);
        // 2 * P
        ea.shift(remCtxt, 1);
        ea.decrypt(remCtxt, secretKey, result);
        cout << i << "th 2 * P = " << result << endl;
        result.clear();
        // 2 * P - D
        subtract(tempCtxt, signCtxt, remCtxt, tempDenCtxt, numLength, ea);

        ea.decrypt(tempCtxt, secretKey, result);
        cout << i << "th subtract = " << result << endl;
        result.clear();

        // if signCtxt is 1 (2 * P - D > 0), then q_i = 1
        // if signCtxt is 0 (2 * P - D < 0), then q_i = 0
        Ctxt extractSignCtxt = signCtxt;
        extractSignCtxt.multByConstant(signPoly);
        quoCtxt.addCtxt(extractSignCtxt);
        
        ea.decrypt(quoCtxt, secretKey, result);
        cout << i << "th quotient = " << result << endl;
        result.clear();

        // if signCtxt is 0, then restore P
        tempCtxt.multiplyBy(signCtxt);
        complement(signCtxt, signCtxt, ea);
        remCtxt.multiplyBy(signCtxt);
        remCtxt.addCtxt(tempCtxt);

        cout << endl;
    }
    ea.shift(remCtxt, -numLength / 2);
}


void rationalToCF(vector<Ctxt>& cfCtxt, const Ctxt& numCtxt, const Ctxt& denCtxt, const long& denBitSize, const EncryptedArray& ea){
    assert(&numCtxt.getPubKey() == &denCtxt.getPubKey());

    const FHEPubKey&    publicKey = denCtxt.getPubKey();
    long                numPQ = 2 * denBitSize;
    vector<Ctxt>        remCtxt(numPQ + 2, publicKey);

    cfCtxt.clear();
    cfCtxt.resize(numPQ, publicKey);

    remCtxt[0] = numCtxt;
    remCtxt[1] = denCtxt;

    for(unsigned long i = 2; i < numPQ + 2; i++){
        // restoringDivision(cfCtxt[i - 2], remCtxt[i], remCtxt[i - 2], remCtxt[i - 1], denBitSize, ea);
    }
}