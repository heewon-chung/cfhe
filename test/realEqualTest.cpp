#include <cstdlib>
#include <vector>

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generateMsg.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(int argc, char* argv[]){
	if(argc != 4){
        cout << "\nplease enter three parameter for test...\n";
        cout << "such as numPQ, lengthPQ, L...\n\n";

        return -1;
    }

	cout << "\nHomomorphic Equality Test over the Real Numbers Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
	long d = 0;
	long c = 3;
    long L = 0;
    long lengthPQ = 0;
    long numPQ = 0;
    
    if(argc > 1){
        numPQ = atoi(argv[1]);
		lengthPQ = atoi(argv[2]);
		L = atoi(argv[3]);
    }
    
    long m = FindM(security, L, c, p, d, 0, 0);
    FHEcontext context(m, p, r);
    buildModChain(context, L);
    
    ZZX F = context.alMod.getFactorsOverZZ()[0];

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;
    secretKey.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);
    long numSlots = ea.size();

    cout << endl;
    printSettings(L, numPQ, lengthPQ);
    
    RR                      Msg1, Msg2;
    vector<vector<long>>    message1, message2;
    vector<long>            equalResult;
    vector<Ctxt>            ct1(numPQ, publicKey), ct2(numPQ, publicKey);
    Ctxt                    equalCt(publicKey);
    
    generateProblemInstance(message1, numSlots, numPQ, lengthPQ);
    generateProblemInstance(message2, numSlots, numPQ, lengthPQ);
    
    cout << endl << "Msg1 (poly) = ";
    Msg1 = printAndReconstructNum(message1, numPQ, lengthPQ);
    cout << "Msg2 (poly) = ";
    Msg2 = printAndReconstructNum(message2, numPQ, lengthPQ);
	
	cout << "Msg1 (val): " << Msg1 << endl;
	cout << "Msg2 (val): " << Msg2 << endl;
    
    for(unsigned long i = 0; i < numPQ; i++){
        ea.encrypt(ct1[i], publicKey, message1[i]);
        ea.encrypt(ct2[i], publicKey, message2[i]);
    }
    
	// timers
	TIMER start;
	TIMER end;
	start = TIC;
    equalityTestOverR(equalCt, ct1, ct2, lengthPQ, ea);
	end = TOC;
	cout << "\nTime per reals Equality test Evaluation time: " << get_time_us(start, end, 1) << " microsec" << endl;
	
    ea.decrypt(equalCt, secretKey, equalResult);

    cout << "\nEqual Result (Plain): " << (Msg1 == Msg2) << endl;
    cout << "Equal Result (Encrypted): " << equalResult[0] << endl;
    cout << "Equal Levels Left: " << equalCt.findBaseLevel() << endl;
	
    return 0;
}