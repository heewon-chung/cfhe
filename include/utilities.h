#ifndef SIMDUTILITIES_H
#define SIMDUTILITIES_H

#include <vector>
#include <cassert>
#include <chrono>

#include "NTL/ZZX.h"

#include <helib/EncryptedArray.h>
#include <helib/Ctxt.h>

using namespace std;
using namespace helib;

#define TIMER std::chrono::time_point<std::chrono::steady_clock>
#define TIC chrono::steady_clock::now()
#define TOC chrono::steady_clock::now()

void ctxtSum(helib::Ctxt&, const helib::Ctxt&, const long, const helib::EncryptedArray&);
void ctxtProduct(helib::Ctxt&, const helib::Ctxt&, const long, const helib::EncryptedArray&);
void reverseCtxtProduct(helib::Ctxt&, const helib::Ctxt&, const long, const helib::EncryptedArray&);
void fillAllSlots(helib::Ctxt&, const helib::Ctxt&, const vector<long>&, const helib::EncryptedArray&);

void printSettings(long, long, long, long, long);
void printSettings(long, long, long);

ZZ evalPoly( ZZX & poly, ZZ point );

double get_time_us( std::chrono::time_point<std::chrono::steady_clock> & start, 
					std::chrono::time_point<std::chrono::steady_clock> & end, 
					uint32_t N=1);

void addTree(vector<helib::Ctxt>&, helib::Ctxt&);
void mulTree(vector<helib::Ctxt>&, helib::Ctxt&);

#endif