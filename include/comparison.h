#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "utilities.h"

void equalityTestoverZ(Ctxt&, const Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void equalityTestoverR(Ctxt&, const Ctxt&, const Ctxt&, const long, const long, const EncryptedArray&, const FHESecKey&);
void comparisonTestoverZ(Ctxt&, const Ctxt&, const Ctxt&, const bool, const long, const EncryptedArray&);

#endif