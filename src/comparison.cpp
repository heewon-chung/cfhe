#include "comparison.h"

using namespace std;
using namespace NTL;

// Equality Test over the Integers
void eqTestoverZ(Ctxt& resultCt, const Ctxt& ct1, const Ctxt& ct2, const Ctxt& oneCtxt, const long numLength, const EncryptedArray& ea){
    Ctxt tempCtxt = ct1;
    resultCt = ct2;

    tempCtxt.addCtxt(resultCt);
    tempCtxt.addCtxt(oneCtxt);

    ctxtProduct(resultCt, tempCtxt, numLength, ea);
}

// Greater-Than Test over the Integers

// Less-Than Test over the Integers