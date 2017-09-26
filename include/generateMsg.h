#ifndef GENERATEMSG_H
#define GENERATEMSG_H

#include <vector>
#include <cassert>

#include "NTL/ZZ.h"
#include "NTL/RR.h"

using namespace std;
using namespace NTL;

void generateProblemInstance(vector<long>&, const long, const long);
void generateProblemInstance(vector<vector<long>>&, const long, const long, const long);
ZZ printAndReconstructNum(vector<long>&, const long);
RR printAndReconstructNum(vector<vector<long>>&, const long, const long);

#endif