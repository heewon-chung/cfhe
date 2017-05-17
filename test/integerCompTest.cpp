#include <cstdlib>
#include <vector>

//#include "../../../Library/HElib-master/src/EncryptedArray.h"
//#include "../../../Library/HElib-master/src/Ctxt.h"

#include "EncryptedArray.h"
#include "Ctxt.h"

#include "generalTools.h"
#include "utilities.h"
#include "comparison.h"

using namespace std;
using namespace NTL;

int main(){
	
	cout << "Homomorphic Integers Comparison Test Started...\n";
	
    srand(time(NULL));
    SetSeed(to_ZZ(time(NULL)));

    long p = 2;
    long r = 1;
    long security = 64;
    long m = 6361;
    long L = 10;
    long currentLength = 6;

    FHEcontext context(m, p, r);
    buildModChain(context, L);
    ZZX F = context.alMod.getFactorsOverZZ()[0];
	const EncryptedArray dummy(context, F);
	long numSlots = dummy.size();
	
	printSettings( p, r, security, m, L, numSlots );
	cout << "plaintext numbers bit-length: " << currentLength << endl;

    // Secret & Public Key Generation
    cout << "Generating Keys... \n";
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;
    secretKey.GenSecKey(security);

    cout << "Generating Frobenius and SIMD Matrices... \n";
    addFrbMatrices(secretKey);
    addSome1DMatrices(secretKey);

    const EncryptedArray ea(context, F);
    
    ZZ Msg1, Msg2;
    vector<ZZX> message1, message2, compResult;
    
    Ctxt ct1(publicKey), ct2(publicKey), compCt(publicKey);

    generateProblemInstance(message1, ea.size(), currentLength);
    generateProblemInstance(message2, ea.size(), currentLength);
    
    cout << endl << "Msg1 (poly) = ";
    Msg1 = printAndReconstructNum(message1, currentLength);
    cout << "Msg2 (poly) = ";
    Msg2 = printAndReconstructNum(message2, currentLength);
	
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
    comparisonTestOverZ(compCt, ct1, ct2, lessThan, currentLength, ea);
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

	cout << "Homomorphic Integers Comparison Test Terminated...\n";
    return 0;
}