#ifndef GOSPER_H
#define GOSPER_H

#include <cstdlib>
#include <vector>
#include <cassert>

#include "EncryptedArray.h"
#include "Ctxt.h"
#include "comparison.h"

// z = (numConst + numCoeff * x) / (denConst + denCoeff * x)
typedef struct cfObject{
    Ctxt numConst;
    Ctxt numCoeff;
    Ctxt denConst;
    Ctxt denCoeff;
} CFObject;

inline void intake(cfObject&, const Ctxt&);
inline void intakeInfty(cfObject&);
inline void outtake(Ctxt&, const cfObject&, const long, const EncryptedArray&);
void gosper(vector<Ctxt>&, const vector<Ctxt>&, cfObject&, const long, const EncryptedArray&);

#endif