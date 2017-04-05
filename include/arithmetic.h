#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <cstdlib>
#include <vector>
#include <cassert>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "utilities.h"

void fullAdder(Ctxt&, const Ctxt&, const Ctxt&, long&, const EncryptedArray&);
void complement(Ctxt&, const Ctxt&, const long&, const EncryptedArray&);
void subtract(Ctxt&, const Ctxt&, const Ctxt&, long&, const EncryptedArray&);

#endif