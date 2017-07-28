#include "gosper.h"

using namespace std;
using namespace NTL;

inline void intake(cfObject& object, const Ctxt& pq){
    Ctxt temp1 = object.numConst;
    Ctxt temp2 = object.denConst;

    object.numConst = object.numCoeff;
    object.denConst = object.denCoeff;

    object.numCoeff.multiplyBy(pq);
    object.denCoeff.multiplyBy(pq);

    object.numCoeff.addCtxt(temp1);
    object.denCoeff.addCtxt(temp2);
}

inline void intakeInfty(cfObject& object){
    object.numConst = object.numCoeff;
    object.denConst = object.denCoeff;
}

inline void outtake(Ctxt& rem, const cfObject& object, const long numLength, const EncryptedArray& ea){
    const FHEPubKey& publicKey = object.numCoeff.getPubKey();
    bool lessThan = 1;
    rem = object.numCoeff;

    for(unsigned long i = 0; i < numLength; i++){
        Ctxt isLess(publicKey);
        comparisonTestOverZ(isLess, rem, object.denCoeff, lessThan, numLength, ea);
        isLess.multiplyBy(object.denCoeff);
        rem -= isLess;
    }
}

void gosper(vector<Ctxt>& outputPQ, const vector<Ctxt>& inputPQ, cfObject& object, const long numLength, const EncryptedArray& ea){
    for(unsigned long i = 0; i < inputPQ.size(); i++){
        intake(object, inputPQ[i]);
    }

    // intakeInfty(object);

    for(unsigned long i = 0; i < numLength; i++){
        outtake(outputPQ[i], object, numLength, ea);
    }
}