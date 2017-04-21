#ifndef QUERIES_H
#define QUERIES_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "comparison.h"
#include "EncryptedDatabase.h"

void searchQuery(EncryptedData&, const vector<Ctxt>&, const EncryptedDB&, const EncryptedArray&);
void searchAndComputeQuery(EncryptedData&, const vector<Ctxt>&, const EncryptedDB&, const EncryptedArray&);
    
#endif