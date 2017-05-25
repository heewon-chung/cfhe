//
// Created by btan on 8/12/16.
//
#include "generalTools.h"

using namespace std;
using namespace NTL;


long vector2Long(vector<ZZX>& message, long numLength) {
    long query = 0;
    for(unsigned long i = 0; i < numLength; i++) {
        if(message[i] == ZZX(1)) {
            query += (1L << i);
        }
    }

    return query;
}


long vector2Long(vector<long>& message, long numLength) {
    long query = 0;
    for(unsigned long i = 0; i < numLength; i++) {
        if(message[i] == 1) {
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


void generateProblemInstance(vector<vector<ZZX>>& message, long numSlots, long numPQ, long lengthPQ) {
    assert(numPQ * lengthPQ <= numSlots);
    message.clear();
    message.resize(numPQ);
    
    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            message[i].push_back(ZZX(rand() % 2));
        }
        message[i].resize(numSlots, ZZX(0));
    }
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


RR printAndReconstructNum(vector<vector<ZZX>>& message, long numPQ, long lengthPQ) {
    RR Msg = RR(0);
    vector<ZZ> msg(numPQ, ZZ(0));

    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            cout << message[i][j] << " ";
            if(message[i][j] == ZZX(1)){
                msg[i] += ZZ((1L << j));
            }
        }
        cout << "\t";
    }
    cout << endl;

    for(int i = numPQ - 1; i >= 0; i--){ 
        Msg += conv<RR>(msg[i]);
        if(i != 0){
            Msg = 1 / Msg;
        }
    }
    
    return Msg;
}