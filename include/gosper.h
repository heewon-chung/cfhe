#ifndef GOSPER_H
#define GOSPER_H

#include <cstdlib>
#include <vector>
#include <cassert>

#include <helib/EncryptedArray.h>
#include <helib/Ctxt.h>
#include "comparison.h"

using namespace std;
using namespace helib;

// z = (numConst + numCoeff * x) / (denConst + denCoeff * x)
typedef struct cfObject{
    helib::Ctxt numConst;
    helib::Ctxt numCoeff;
    helib::Ctxt denConst;
    helib::Ctxt denCoeff;
} CFObject;

inline void intake(cfObject&, const helib::Ctxt&);
inline void intakeInfty(cfObject&);
inline void outtake(helib::Ctxt&, const cfObject&, const long, const helib::EncryptedArray&);
void gosper(vector<helib::Ctxt>&, const vector<helib::Ctxt>&, cfObject&, const long, const helib::EncryptedArray&);

#endif