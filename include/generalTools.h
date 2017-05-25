//
// Created by btan on 8/12/16.
//
#ifndef GENERALTOOLS_H
#define GENERALTOOLS_H

#include <vector>
#include <cassert>

#include "NTL/ZZX.h"
#include "NTL/RR.h"

using namespace std;
using namespace NTL;

long vector2Long(vector<ZZX>&, long);
long vector2Long(vector<long>&, long);
vector<long> integer2Vector(int);
void generateProblemInstance(vector<ZZX>&, long, long);
void generateProblemInstance(vector<vector<ZZX>>&, long, long, long);
ZZ printAndReconstructNum(vector<ZZX>&, long);
RR printAndReconstructNum(vector<vector<ZZX>>&, long, long);

#endif