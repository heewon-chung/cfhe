#ifndef COMPARISON_H
#define COMPARISON_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "comparison.h"

void searchQuery(Ctxt&, const vector<Ctxt>&, const vector<vector<Ctxt>>&, const long&, const EncryptedArray&);
void searchAndComputeQuery(Ctxt&, Ctxt&, const vector<Ctxt>&, const vector<vector<Ctxt>>&, vector<vector<Ctxt>>&, const bool, const long&, const EncryptedArray&);
