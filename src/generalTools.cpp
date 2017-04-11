#include "generalTools.h"

using namespace std;
using namespace NTL;


long vector2Long(vector<ZZX>& result, long numLength) {
    long query = 0;
    for(unsigned long i = 0; i < numLength; i++) {
        if(result[i] == ZZX(1)) {
            query += (1L << i);
        }
    }

    return query;
}


void generateProblemInstance(vector<ZZX>& message, long numSlots, long currentLength) {
    message.clear();

    for(unsigned long i = 0; i < currentLength; i++) {
        message.push_back(ZZX(rand() % 2));
    }

    message.resize(numSlots, ZZX(0));
}


void generateProblemInstance(vector<ZZX>& message, long numSlots, long numPQ, long lengthPQ) {
    message.clear();

    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            message.push_back(ZZX(rand() % 2));
        }
    }

    message.resize(numSlots, ZZX(0));
}


ZZ printAndReconstructNum(vector<ZZX>& message, long numLength) {
    ZZ Msg = ZZ(0);
    for (unsigned long i = 0; i < numLength; i++) {
        cout << message[i] << ", ";
        if (message[i] == ZZX(1)) {
            Msg += ZZ((1L << i));
        }
    }
    cout << endl;
    return Msg;
}

vector<ZZ> printAndReconstructNum(vector<ZZX>& message, long numPQ, long lengthPQ) {
    vector<ZZ> Msg(numPQ, ZZ(0));

    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            cout << message[i * numPQ + j] << " ";
            if(message[i * numPQ + j] == ZZX(1)){
                Msg[i] += ZZ((1L << j));
            }
        }
        cout << "\t";
    }
    cout << endl;
    return Msg;
}