#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>
//
//#include "../../../Library/HElib-master/src/EncryptedArray.h"
//#include "../../../Library/HElib-master/src/Ctxt.h"

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "utilities.h"

void equalityTestOverZ(Ctxt&, const Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void equalityTestOverR(Ctxt&, const vector<Ctxt>&, const vector<Ctxt>&, const long, const EncryptedArray&);
void comparisonTestOverZ(Ctxt&, const Ctxt&, const Ctxt&, const bool, const long, const EncryptedArray&);
void comparisonTestOverR(Ctxt&, const vector<Ctxt>&, const vector<Ctxt>&, const bool, const long, const EncryptedArray&);

#endif