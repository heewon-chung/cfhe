#ifndef SORTING_H
#define SORTING_H

#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>

#include <helib/EncryptedArray.h>
#include <helib/Ctxt.h>
#include "utilities.h"
#include "comparison.h"
#include "arithmetic.h"
#include "generalTools.h"


using namespace std;
using namespace helib;

void swap(vector<helib::Ctxt>&, vector<helib::Ctxt>&, const vector<helib::Ctxt>&, const vector<helib::Ctxt>&, const long, const helib::EncryptedArray&);
void directSort(vector<vector<helib::Ctxt>>&, vector<vector<helib::Ctxt>>&, const long, const helib::EncryptedArray&);

#endif