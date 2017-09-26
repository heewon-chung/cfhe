#ifndef GENERATEMSG_H
#define GENERATEMSG_H

#include <vector>
#include <cassert>

#include "NTL/ZZ.h"
#include "NTL/RR.h"

using namespace std;
using namespace NTL;

void generateProblemInstance(vector<long>&, long, long);
void generateProblemInstance(vector<vector<long>>&, long, long, long);
ZZ printAndReconstructNum(vector<long>&, long);
RR printAndReconstructNum(vector<vector<long>>&, long, long);

#endif