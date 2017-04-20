#include "queries.h"
/*
 *  @Input
 *  @Output
 */
void searchQuery(Ctxt& searchCtxt, const vector<Ctxt>& queryCtxt, const vector<vector<Ctxt>>& dbCtxt, const long& lengthPQ, const EncryptedArray& ea){
    assert(&queryCtxt[0].getPubKey() == &dbCtxt[0][0].getPubKey());
    unsigned long dbSize = dbCtxt.size();
    const FHEPubKey& publicKey = queryCtxt[0].getPubKey();
    vector<Ctxt> resultCtxt(dbSize, publicKey);
    vector<long> mask;
    ZZX maskPoly;

    mask.push_back(1);
    mask.resize(ea.size());
    ea.encode(maskPoly, mask);

    for(unsigned long i = 0; i < dbSize; i++){
        equalityTestOverR(resultCtxt[i], queryCtxt, dbCtxt[i], lengthPQ, ea);
        resultCtxt[i].addConstant(maskPoly);
        if(i == 0){
            searchCtxt = resultCtxt[i];
        }
        else{
            searchCtxt.multiplyBy(resultCtxt[i]);
        }
    }

    searchCtxt.addConstant(maskPoly);
}


void searchAndComputeQuery(Ctxt& resultNumCtxt, Ctxt& resultDenCtxt, const vector<Ctxt>& queryCtxt, const vector<vector<Ctxt>>& dbCtxt, vector<vector<Ctxt>>& fracCtxt, const long& lengthpQ, const EncryptedArray& ea){
    unsigned long dbSize = dbCtxt.size();
    assert(fracCtxt[rand() % dbSize].size() == 2);
    const FHEPubKey& publicKey = queryCtxt[0].getPubKey();

    for(unsigned long i = 0; i < dbSize; i++){
        Ctxt resultCtxt(publicKey);
        equalityTestOverR(resultCtxt, queryCtxt, dbCtxt[i], lengthPQ, ea);
        if(i == 0){
            // p_1 = resultNumCtxt
            resultNumCtxt = fracCtxt[i][0];
            // q_1 = resultDenCtxt
            resultDenCtxt = fracCtxt[i][1];
        }
        else{
            // tempNumCtxt = p_2, tempDenCtxt = q_2
            Ctxt tempNumCtxt = fracCtxt[i][0], tempDenCtxt = fracCtxt[i][1];
            // if result = 0, then make a numerator as zero.h
            tempNumCtxt.multiplyBy(resultCtxt);
            // Compute numerator
            // p_1 * q_2
            resultNumCtxt.multiplyBy(tempDenCtxt);
            // p_2 * q_1
            tempNumCtxt.multiplyBy(resultDenCtxt);
            // p_1 * q_2 + p_2 * q_1
            resultNumCtxt.addCtxt(tempNumCtxt);
            // Compute denomiator: q_1 * q_2
            resultDenCtxt.multiplyBy(tempDenCtxt);
        }
    }
}