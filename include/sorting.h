#ifndef SORTING_H
#define SORTING_H

#include <cstdlib>
#include <vector>
#include <cassert>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "utilities.h"
#include "comparison.h"

void swap(vector<Ctxt>&, vector<Ctxt>&, const vector<Ctxt>&, const vector<Ctxt>&, const long, const EncryptedArray&);

#endif