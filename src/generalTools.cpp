#include "generalTools.h"

using namespace std;
using namespace NTL;

long vector2Long(vector<ZZX> &result, long numLength) {
    long query = 0;
    for(unsigned long i = 0; i < numLength; i++) {
        if(result[i] == ZZX(1)) {
            query += (1L << i);
        }
        else if(result[i] == ZZX(2)) {
            query -= (1L << i);
        }
    }

    return query;
}

void generateProblemInstance(vector<ZZX> &numberOne, long numSlots, long currentLength) {
    numberOne.clear();

    for(unsigned long i = 0; i < currentLength; i++) {
        numberOne.push_back(ZZX(rand() % 2));
    }

    numberOne.resize(numSlots, ZZX(0));
}

ZZ printAndReconstructNum(vector<ZZX> &numberOne, long numLength) {
    ZZ zzOne = ZZ(0);
    cout << endl;
    for (unsigned long i = 0; i < numLength; i++) {
        cout << numberOne[i] << ", ";
        if (numberOne[i] == ZZX(1)) {
            zzOne += ZZ((1L << i));
        }
        if (numberOne[i] == ZZX(2)) {
            zzOne -= ZZ((1L << i));
        }
    }

    return zzOne;
}