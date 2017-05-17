#include <cstdlib>
#include <vector>

//#include "../../../Library/HElib-master/src/EncryptedArray.h"
//#include "../../../Library/HElib-master/src/Ctxt.h"
#include "EncryptedArray.h"
#include "Ctxt.h"

#include "NTL/RR.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(int argc, char* argv[])
{
	
	cout << "Homomorphic Reals Comparison Test Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
    long m = 0;
    long L = 9;
	long d = 0;
	long c = 3;
    long numPQ = 3;
    long lengthPQ = 4;

	if (argc > 1 )
	{
		lengthPQ = atoi(argv[1]);
		numPQ = atoi(argv[2]);
		L = atoi(argv[3]);
	}

	m = FindM(security, L, c, p, d, 0, 0);
    FHEcontext context(m, p, r);
    buildModChain(context, L);
    ZZX F = context.alMod.getFactorsOverZZ()[0];
	const EncryptedArray dummy(context, F);
	long numSlots = dummy.size();
	
	printSettings( p, r, security, m, L, numSlots );
	cout << "number of partial qutients : " << numPQ << endl;
	cout << "length of each partial qutient : " << lengthPQ << endl;

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;
    secretKey.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);
    numSlots = ea.size();
    
    RR Msg1, Msg2;
    vector<vector<ZZX>> message1, message2;
    vector<long> compResult;
    vector<Ctxt> ct1(numPQ, publicKey), ct2(numPQ, publicKey);
    Ctxt compCt(publicKey);
    
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
	const bool lessThan = 1;
	string str;
	if (lessThan == 1)
		str = "Less Than";
	else
		str = "Greater Than";
	TIMER start;
	TIMER end;
	start = TIC;
    comparisonTestOverR(compCt, ct1, ct2, lessThan, lengthPQ, ea);
	end = TOC;
	cout << "Time per reals comparison (" << str << ") test: " << std::endl;
	cout << "Evaluation time: " << get_time_us(start, end, 1) << " microsec" << endl;
    
    ea.decrypt(compCt, secretKey, compResult);

    cout << endl;
    cout << "Equal Result (Plain): ";
    if(lessThan){
        cout << (Msg1 < Msg2) << endl;
    }
    else {
        cout << (Msg1 > Msg2) << endl;
    }
    cout << "Equal Result (Encrypted): " << compResult[0] << endl;
    cout << "Equal Levels Left: " << compCt.findBaseLevel() << endl;

	cout << "Homomorphic Reals Comparison Test Terminated...\n";
	
    return 0;
}