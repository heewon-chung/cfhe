#ifndef SORTING_H
#define SORTING_H

#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>

#include <helib/EncryptedArray.h>
#include <helib.Ctxt.h>

#include "utilities.h"
#include "comparison.h"
#include "arithmetic.h"
#include "generalTools.h"

void swap(vector<Ctxt>&, vector<Ctxt>&, const vector<Ctxt>&, const vector<Ctxt>&, const long, const EncryptedArray&);
void directSort(vector<vector<Ctxt>>&, vector<vector<Ctxt>>&, const long, const EncryptedArray&);

#endif