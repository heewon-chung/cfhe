#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>

#include <helib/EncryptedArray.h>
#include <helib/FHE.h>
#include "utilities.h"

using namespace std;
using namespace helib;
using namespace NTL;

void equalityTestOverZ(helib::Ctxt&, const helib::Ctxt&, const helib::Ctxt&, const long, const helib::EncryptedArray&);
void equalityTestOverR(helib::Ctxt&, const vector<helib::Ctxt>&, const vector<helib::Ctxt>&, const long, const helib::EncryptedArray&);
void comparisonTestOverZ(helib::Ctxt&, const helib::Ctxt&, const helib::Ctxt&, const bool, const long, const helib::EncryptedArray&);
void comparisonTestOverR(helib::Ctxt&, const vector<helib::Ctxt>&, const vector<helib::Ctxt>&, const bool, const long, const helib::EncryptedArray&);

#endif