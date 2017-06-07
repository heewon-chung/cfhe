#include <cstdlib>
#include <vector>
#include <ctime>

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(void){
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long                    p = 2;
    long                    r = 1;
    long                    security = 80;
	long                    d = 0;
	long                    c = 3;
    long                    L = 7;
	long                    m = FindM(security, L, c, p, d, 0, 0);
    FHEcontext              context(m, p, r);
    buildModChain(context, L);
    ZZX                     F1 = context.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey               secretKey(context);
    const FHEPubKey&        publicKey = secretKey;
    secretKey.GenSecKey(security); 

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);     
    addSome1DMatrices(secretKey); 

    const EncryptedArray    ea(context, F1);

    /* Test Set for Message Encoding
     * 1. [7;5,6,1] = 7.194, 7.1944, 7.19444 (lengthPQ = 3)
     * 2. [6;3,5,3,2] = 6.3135, 6.31355, 6.313669, 6.3135593 (lengthPQ = 3)
     * 3. [15;3,10,6,1,2] = 15.3227, 15.32273, 15.3227344 (lengthPQ = 4)
    */ 
    
    // Continued Fraction
    vector<long>            pq; 
    vector<vector<long>>    pqVector;
    pq.push_back(6);    pq.push_back(3);    pq.push_back(5);    pq.push_back(3);     pq.push_back(2);
    
    int numPQ = pq.size(),
        lengthPQ = 3;

    for(unsigned long i = 0; i < numPQ; i++){
        pqVector.push_back(integer2Vector(pq[i]));
        pqVector[i].resize(ea.size());
    }

    Ctxt            resultReal(publicKey);
    vector<Ctxt>    pqMsg1(numPQ, publicKey),
                    pqMsg2(numPQ, publicKey);

    for(unsigned long i = 0; i < numPQ; i++){
        ea.encrypt(pqMsg1[i], publicKey, pqVector[i]);
        ea.encrypt(pqMsg2[i], publicKey, pqVector[i]);
    }

    TIMER start;
	TIMER end;
    start = TIC;
    equalityTestOverR(resultReal, pqMsg1, pqMsg2, lengthPQ, ea);
    end = TOC;

    cout << "Equality Evaluation time for CF: " << get_time_us(start, end, 1) / 1000000 << " sec" << endl;

    return 0;
}
