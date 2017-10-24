#include <cstdlib>
#include <vector>

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "NTL/RR.h"

#include "generateMsg.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(int argc, char* argv[])
{
    if(argc != 4){
        cerr << "\nplease enter three parameter for test\n";
        cerr << "such as lengthPQ, numPQ and L...\n\n";

        return -1;
    }

	cout << "\nHomomorphic Comparison Test over the Real Numbers Started...\n";
    
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
        lengthPQ = atoi(argv[1]);
		numPQ = atoi(argv[2]);
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
    
    // Variables for Messages
    RR                      Msg1, Msg2;
    vector<vector<long>>    message1, message2;
    vector<long>            compResult;

    // Variables for Encryption
    vector<Ctxt>            ct1(numPQ, publicKey), ct2(numPQ, publicKey);
    Ctxt                    compCt(publicKey);

    // Timer
    TIMER start;
    TIMER end;
    
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
	
	const bool lessThan = 1;
	string str;
	if (lessThan == 1){
        str = "Less Than";
    }
	else{
        str = "Greater Than";
    }
    
	start = TIC;
    comparisonTestOverR(compCt, ct1, ct2, lessThan, lengthPQ, ea);
	end = TOC;
	cout << "\nTime per Reals Comparison (" << str << ") Evaluation Time: " << get_time_us(start, end, 1) << " microsec" << endl;
    
    ea.decrypt(compCt, secretKey, compResult);

    cout << "\nComparison Result (Plain): ";
    if(lessThan){
        cout << (Msg1 < Msg2) << endl;
    }
    else {
        cout << (Msg1 > Msg2) << endl;
    }
    cout << "Comparison Result (Encrypted): " << compResult[0] << endl;
    cout << "Comparison Levels Left: " << compCt.findBaseLevel() << endl;
	
    return 0;
}