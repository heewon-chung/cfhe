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
using namespace helib;

long vector2Long(vector<ZZX>&, long);
long vector2Long(vector<long>&, long);
vector<long> integer2Vector(int);
vector<long> integer2Vector(ZZ, long);
vector<long> vector2Vector(vector<long>, int);

#endif