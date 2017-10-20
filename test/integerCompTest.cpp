#include <cstdlib>
#include <vector>

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generateMsg.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(){
	
	cout << "Homomorphic Comparison Test over the Integers Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 80;
	long d = 0;
	long c = 3;
    long L = 8;
    long bitLength = 10;
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
    // printSettings( p, r, security, m, L, numSlots );
	cout << "plaintext numbers bit-length: " << bitLength << endl;
    
    ZZ              Msg1, Msg2;
    vector<long>    message1, message2, compResult;
    
    Ctxt ct1(publicKey), ct2(publicKey), compCt(publicKey);

    generateProblemInstance(message1, numSlots, bitLength);
    generateProblemInstance(message2, numSlots, bitLength);
    
    cout << endl << "Msg1 (poly) = ";
    Msg1 = printAndReconstructNum(message1, bitLength);
    cout << "Msg2 (poly) = ";
    Msg2 = printAndReconstructNum(message2, bitLength);
	
	cout << "Msg1 (val): " << Msg1 << endl;
	cout << "Msg2 (val): " << Msg2 << endl;
	
    ea.encrypt(ct1, publicKey, message1);
    ea.encrypt(ct2, publicKey, message2);

	// lessThan (0): evaluating greater than circuit
	// lessThan (1): evaluating less than circuit
    bool lessThan = 1; 
	string str;
	if (lessThan == 1)
		str = "Less Than";
	else
		str = "Greater Than";
    
	// timers
	TIMER start;
	TIMER end;
	start = TIC;
    comparisonTestOverZ(compCt, ct1, ct2, lessThan, bitLength, ea);
	end = TOC;
	cout << "Time per integers comparison (" << str << ") test: " << get_time_us(start, end, 1) << " microsec" << std::endl;

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

	cout << "Homomorphic Comparison Test over the Integers Terminated...\n";
    return 0;
}