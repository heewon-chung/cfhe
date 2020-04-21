#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>

#include "../../helib_install/helib_pack/include/EncryptedArray.h"
#include "../../helib_install/helib_pack/include/FHE.h"
#include "utilities.h"

void equalityTestOverZ(Ctxt&, const Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void equalityTestOverR(Ctxt&, const vector<Ctxt>&, const vector<Ctxt>&, const long, const EncryptedArray&);
void comparisonTestOverZ(Ctxt&, const Ctxt&, const Ctxt&, const bool, const long, const EncryptedArray&);
void comparisonTestOverR(Ctxt&, const vector<Ctxt>&, const vector<Ctxt>&, const bool, const long, const EncryptedArray&);

#endif