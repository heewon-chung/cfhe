#ifndef GENERALTOOLS_H
#define GENERALTOOLS_H

#include <vector>

#include "NTL/ZZX.h"

using namespace std;
using namespace NTL;

long vector2Long(vector<ZZX>&, long);
void generateProblemInstance(vector<ZZX>&, long, long);
ZZ printAndReconstructNum(vector<ZZX>&, long);

#endif