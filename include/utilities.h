#ifndef SIMDUTILITIES_H
#define SIMDUTILITIES_H

#include <vector>
#include <cassert>

#include "NTL/ZZX.h"
//#include "../../../Library/HElib-master/src/EncryptedArray.h"

#include "EncryptedArray.h"


void ctxtSum(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void ctxtProduct(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void reverseCtxtProduct(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void fillAllSlots(Ctxt&, const Ctxt&, const vector<long>&, const EncryptedArray&);

#endif