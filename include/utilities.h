#ifndef SIMDUTILITIES_H
#define SIMDUTILITIES_H

#include <vector>
#include <cassert>
#include <chrono>

#include "NTL/ZZX.h"

#include "EncryptedArray.h"

#define TIMER std::chrono::time_point<std::chrono::steady_clock>
#define TIC chrono::steady_clock::now()
#define TOC chrono::steady_clock::now()

void ctxtSum(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void ctxtProduct(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void reverseCtxtProduct(Ctxt&, const Ctxt&, const long, const EncryptedArray&);
void fillAllSlots(Ctxt&, const Ctxt&, const vector<long>&, const EncryptedArray&);

void printSettings( long p, long r, long security, long m, long L, long numSlots );

ZZ evalPoly( ZZX & poly, ZZ point );

double get_time_us( std::chrono::time_point<std::chrono::steady_clock> & start, 
					std::chrono::time_point<std::chrono::steady_clock> & end, 
					uint32_t N=1);

void mulTree(vector<Ctxt> &inputs, Ctxt &ret);

#endif