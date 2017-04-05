#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "utilities.h"

void eqTestoverZ(Ctxt&, const Ctxt&, const Ctxt&, const Ctxt&, const long, const EncryptedArray&);

#endif