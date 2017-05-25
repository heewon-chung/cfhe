#include "utilities.h"

using namespace std;
using namespace NTL;


// add all messages in a ciphertext
void ctxtSum(Ctxt& addCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());

    Ctxt    tempCtxt1 = ct, 
            tempCtxt2 = ct;

    long shiftAmt = 1;
    while(shiftAmt < numLength){
        ea.shift(tempCtxt2, -shiftAmt);
        tempCtxt2.addCtxt(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    addCt = tempCtxt1;
}


// product messages in a ciphertext
// First entry contains information
void ctxtProduct(Ctxt& mulCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea){
    assert(numLength <= ea.size());
    
    Ctxt    tempCtxt1 = ct, 
            tempCtxt2 = ct;
    long    shiftAmt = 1;

    while(shiftAmt < numLength){
        vector<long> mask(numLength - shiftAmt);
        
        mask.resize(numLength, 1);
        mask.resize(ea.size());
        ZZX maskPoly;
        ea.encode(maskPoly, mask);

        ea.shift(tempCtxt2, -shiftAmt);
        tempCtxt2.addConstant(maskPoly);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    mulCt = tempCtxt1;
}

// last entry contains information
void reverseCtxtProduct(Ctxt& mulCt, const Ctxt& ct, const long numLength, const EncryptedArray& ea) {
    assert(numLength <= ea.size());
    
    Ctxt    tempCtxt1 = ct, 
            tempCtxt2 = ct;
    long    shiftAmt = 1;

    while (shiftAmt < numLength) {
        vector<long>    mask(shiftAmt, 1);
        ZZX             maskPoly;
        
        mask.resize(ea.size());
        ea.encode(maskPoly, mask);
        
        ea.shift(tempCtxt2, shiftAmt);
        tempCtxt2.addConstant(maskPoly);
        tempCtxt2.multiplyBy(tempCtxt1);
        tempCtxt1 = tempCtxt2;
        shiftAmt *= 2;
    }

    mulCt = tempCtxt1;
}


void fillAllSlots(Ctxt& filledCtxt, const Ctxt& orgCtxt, const vector<long>& filledPositions, const EncryptedArray& ea) {
    assert(filledPositions.size() <= ea.size());
    
    Ctxt    tempCtxt(orgCtxt.getPubKey());
    long    firstPos = 0,
            lastPos = filledPositions.size() - 1,
            totalLength = filledPositions.size();

    filledCtxt = orgCtxt;
    while (filledPositions[firstPos] != 1) {
        firstPos++;
    }
    ea.shift(filledCtxt, -firstPos);

    while (filledPositions[lastPos] != 1) {
        lastPos--;
    }

    long range = lastPos - firstPos + 1,
         currentLength = range;
    
    while (currentLength < totalLength) {
        tempCtxt = filledCtxt;
        ea.shift(tempCtxt, currentLength);
        filledCtxt += tempCtxt;
        currentLength *= 2;
    }
}

void printSettings( long p, long r, long security, long m, long L, long numSlots)
{
	cout << "================== System Settings ==================\n";
	cout << "plaintext modulus\t\t(p): " << p << endl;
	cout << "(lifting of p^r\t\t\t(r): " << r << endl;
	cout << "security level\t\t\t(k): " << security << endl;
	cout << "m-th cycltomic poly\t\t(m): " << m << endl;
	cout << "max multiplication depth\t(L): " << L << endl;
	cout << "number of slots\t\t\t(s): " << numSlots << endl;
	cout << "=====================================================\n";
}

#include <chrono>
// get time in micro seconds for invoking a function (N) times
// start: starting time
// end: end time
// N : number of repetitions 
double get_time_us( std::chrono::time_point<std::chrono::steady_clock> & start, 
					std::chrono::time_point<std::chrono::steady_clock> & end, 
					uint32_t N)
{
  auto diff = end-start;
  return (long double)(std::chrono::duration_cast<std::chrono::microseconds>(diff).count())/N;
}

// eval polynomial at a point
ZZ evalPoly( ZZX & poly, ZZ point )
{
	ZZ res = to_ZZ(0);
	for (int i = 0; i < poly.rep.length(); i++)
	{
		res = res + poly[i]*power(point, i);
	}
}

void mulTree( vector<Ctxt> &inputs, Ctxt &ret)
{
	int original_size = inputs.size();

	if (original_size == 1)
	{
		ret = inputs[0];
		return;
	}

	// multiply two elements and add to the back of the vector
	#pragma omp parallel for
	for (int i = 0; i < inputs.size() - 1; i += 2)
	{
		//inputs.emplace_back(inputs[i]*inputs[i + 1]);
		inputs[i].multiplyBy(inputs[i+1]);
		inputs.emplace_back( inputs[i] );
	}
	
	ret = inputs[inputs.size() - 1]; 

	// remove extra elements
	while (inputs.size() > original_size)
	{
		inputs.erase(inputs.begin() + original_size);
	}
}
