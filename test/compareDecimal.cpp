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
    long                    L1 = 11, L2 = 7;
	long                    m1 = FindM(security, L1, c, p, d, 0, 0),
                            m2 = FindM(security, L2, c, p, d, 0, 0);
    FHEcontext              context1(m1, p, r),
                            context2(m2, p, r);
    buildModChain(context1, L1);        
    buildModChain(context2, L2);
    ZZX                     F1 = context1.alMod.getFactorsOverZZ()[0],
                            F2 = context2.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey               secretKey1(context1),
                            secretKey2(context2);
    const FHEPubKey&        publicKey1 = secretKey1,
                            publicKey2 = secretKey2;
    secretKey1.GenSecKey(security); 
    secretKey2.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey1);         addFrbMatrices(secretKey2);
    addSome1DMatrices(secretKey1);      addSome1DMatrices(secretKey2);

    const EncryptedArray    ea1(context1, F1),
                            ea2(context2, F2);

    //////////// Decimal
    int                     message = 7194254;
    vector<long>            messageVector = integer2Vector(message);
    messageVector.resize(ea1.size());
    
    //////////// Partial Quotient
    vector<long>            pq; 
    vector<vector<long>>    pqVector;
    pq.push_back(7);
    pq.push_back(5);
    pq.push_back(6);
    pq.push_back(1);
    pq.push_back(3);
    pq.push_back(5);

    int numPQ = pq.size();

    for(unsigned long i = 0; i < numPQ; i++){
        pqVector.push_back(integer2Vector(pq[i]));
        pqVector[i].resize(ea2.size());
    }

    Ctxt            Msg1(publicKey1), 
                    Msg2(publicKey1),
                    resultInteger(publicKey1), 
                    resultReal(publicKey2);
    vector<Ctxt>    pqMsg1(numPQ, publicKey2), 
                    pqMsg2(numPQ, publicKey2);

    ea1.encrypt(Msg1, publicKey1, messageVector);
    ea1.encrypt(Msg2, publicKey1, messageVector);

    for(unsigned long i = 0; i < numPQ; i++){
        ea2.encrypt(pqMsg1[i], publicKey2, pqVector[i]);
        ea2.encrypt(pqMsg2[i], publicKey2, pqVector[i]);
    }

    // double t = GetTime();
    // cout << "Integer" << endl;
    equalityTestOverZ(resultInteger, Msg1, Msg2, messageVector.size(), ea1);
    // t = GetTime() - t;
    // cout << "Time = " << t << endl;
    // cout << resultInteger.findBaseLevel() << endl;

    // t = GetTime();
    // cout << "Real" << endl;
    equalityTestOverR(resultReal, pqMsg1, pqMsg2, numPQ, ea2);
    // t = GetTime() - t;
    // cout << "Time = " << t << endl;
    // cout << resultReal.findBaseLevel() << endl;

    return 0;
}
