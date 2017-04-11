#ifndef GENERALTOOLS_H
#define GENERALTOOLS_H

#include <vector>
#include <cassert>

#include "NTL/ZZX.h"

using namespace std;
using namespace NTL;

long vector2Long(vector<ZZX>&, long);
void generateProblemInstance(vector<ZZX>&, long, long);
void generateProblemInstance(vector<ZZX>&, long, long, long);
ZZ printAndReconstructNum(vector<ZZX>&, long);
vector<ZZ> printAndReconstructNum(vector<ZZX>&, long, long);

#endif