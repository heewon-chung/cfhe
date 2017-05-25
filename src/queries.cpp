#include "queries.h"

using namespace std;
using namespace NTL;

// information is contained in the first slot
void searchQuery(Ctxt& searchCtxt, const vector<Ctxt>& queryCtxt, const EncryptedDB& encDB, const EncryptedArray& ea){
    assert(&queryCtxt[0].getPubKey() == &encDB.cfCtxt[0].getPubKey());
    
    const FHEPubKey&    publicKey = queryCtxt[0].getPubKey();
    const EncryptedDB&  dbCFCtxt = encDB.getCFCtxt();
    unsigned long       dbSize = encDB.getDBSize();
    vector<Ctxt>        isEqual(dbSize, publicKey);
    vector<long>        flip;
    ZZX                 flipPoly;

    flip.push_back(1);
    flip.resize(ea.size());
    ea.encode(flipPoly, flip);

    for(unsigned long i = 0; i < dbSize; i++){
        equalityTestOverR(isEqual[i], queryCtxt, dbCFCtxt[i], encDB.getLengthPQ(), ea);
        isEqual[i].addConstant(flipPoly);
        if(i == 0){
            searchCtxt = isEqual[i];
        }
        else{
            searchCtxt.multiplyBy(isEqual[i]);
        }
    }

    searchCtxt.addConstant(flipPoly);
}


void searchAndComputeQuery(EncryptedData& resultCtxt, const vector<Ctxt>& queryCtxt, const EncryptedDB& encDB, const EncryptedArray& ea){
    assert(&queryCtxt[0].getPubKey() == &encDB.cfCtxt[0].getPubKey());
    
    const FHEPubKey&    publicKey = queryCtxt[0].getPubKey();
    const EncryptedDB&  dbCFCtxt = encDB.getCFCtxt();
    const vector<Ctxt>  dbNumCtxt = encDB.getNumCtxt();
    const vector<CtxT>  dbDenCtxt = encDB.getDenCtxt();
    unsigned long       dbSize = encDB.getDBSize();
    
    for(unsigned long i = 0; i < dbSize; i++){
        Ctxt isEqual(publicKey);
        equalityTestOverR(isEqual, queryCtxt, dbCFCtxt[i], encDB.getLengthPQ(), ea);
        if(i == 0){
            // p_1 = NumCtxt
            resultCtxt.SetNumCtxt(dbNumCtxt[i]);
            // q_1 = DenCtxt
            resultCtxt.SetDenCtxt(dbDenCtxt[i]);
        }
        else{
            // p_2 = tempNumCtxt, q_2 = tempDenCtxt
            Ctxt tempNumCtxt = dbNumCtxt[i], tempDenCtxt = dbDenCtxt[i];
            // if result = 0, then make p_2 = 0
            tempNumCtxt.multiplyBy(isEqual);
            // Compute numerator
            // p_1 * q_2
            Ctxt tempCt1 = resultCtxt.getNumCtxt();
            tempCt1.multiplyBy(tempDenCtxt);
            // p_2 * q_1
            tempNumCtxt.multiplyBy(resultCtxt.getDenCtxt());
            // p_1 * q_2 + p_2 * q_1
            tempNumCtxt.addCtxt(tempCt1);
            resultCtxt.SetNumCtxt(tempNumCtxt);
            // Compute denomiator: q_1 * q_2
            tempDenCtxt.multiplyBy(resultCtxt.getDenCtxt());
            resultCtxt.SetDenCtxt(tempDenCtxt());
        }
    }
}