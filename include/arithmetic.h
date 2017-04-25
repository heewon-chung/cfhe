#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <cstdlib>
#include <vector>
#include <cassert>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "utilities.h"

void fullAdder(Ctxt&, const Ctxt&, const Ctxt&, long&, const EncryptedArray&);
inline void complement(Ctxt&, const Ctxt&, const EncryptedArray&);
inline void complement(Ctxt&, const Ctxt&, const long&, const EncryptedArray&);
inline void twoComplement(Ctxt&, const Ctxt&, long&, const EncryptedArray&);
void subtract(Ctxt&, Ctxt&, const Ctxt&, const Ctxt&, const long&, const EncryptedArray&);
void restoringDivision(Ctxt&, Ctxt&, const Ctxt&, const Ctxt&, const long&, const EncryptedArray&, const FHESecKey&);
void rationalToCF(vector<Ctxt>&, const Ctxt&, const Ctxt&, const long&, const EncryptedArray&);

#endif