#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <cstdlib>
#include <vector>
#include <cassert>

//#include "../../../Library/HElib-master/src/EncryptedArray.h"
//#include "../../../Library/HElib-master/src/Ctxt.h"

// #include "../../helib_install/helib_pack/include/helib/EncryptedArray.h"
// #include "../../helib_install/helib_pack/include/helib/Ctxt.h"

using namespace std;
using namespace helib;

#include <helib/EncryptedArray.h>
#include <helib/Ctxt.h>

#include "utilities.h"

void fullAdder(helib::Ctxt&, const helib::Ctxt&, const helib::Ctxt&, long&, const helib::EncryptedArray&);
inline void complement(helib::Ctxt&, const helib::Ctxt&, const helib::EncryptedArray&);
inline void complement(helib::Ctxt&, const helib::Ctxt&, const long&, const helib::EncryptedArray&);
inline void twoComplement(helib::Ctxt&, const helib::Ctxt&, long&, const helib::EncryptedArray&);
void subtract(helib::Ctxt&, helib::Ctxt&, const helib::Ctxt&, const helib::Ctxt&, const long&, const helib::EncryptedArray&);
void restoringDivision(helib::Ctxt&, helib::Ctxt&, const helib::Ctxt&, const helib::Ctxt&, const long&, const helib::EncryptedArray&, const helib::SecKey&);
void rationalToCF(vector<helib::Ctxt>&, const helib::Ctxt&, const helib::Ctxt&, const long&, const helib::EncryptedArray&);

#endif