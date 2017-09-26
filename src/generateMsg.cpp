#include "generateMsg.h"


void generateProblemInstance(vector<long>& message, long numSlots, long bitLength){
    message.clear();

    for(unsigned long i = 0; i < bitLength; i++){
        message.push_back(rand() % 2);
    }

    message.resize(numSlots);
}


void generateProblemInstance(vector<vector<long>>& message, long numSlots, long numPQ, long lengthPQ){
    assert(numPQ * lengthPQ <= numSlots);
    message.clear();
    message.resize(numPQ);

    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            message[i].push_back(rand() % 2);
        }
        message[i].resize(numSlots);
    }
}


ZZ printAndReconstructNum(vector<long>& message, long numLength){
    ZZ Msg = ZZ(0);
    for (unsigned long i = 0; i < numLength; i++) {
        cout << message[i] << ", ";
        if (message[i] == 1) {
            Msg += ZZ((1L << i));
        }
    }
    cout << endl;

    return Msg;
}


RR printAndReconstructNum(vector<vector<long>>& message, long numPQ, long lengthPQ){
    RR Msg = RR(0);
    vector<ZZ> msg(numPQ, ZZ(0));

    for(unsigned long i = 0; i < numPQ; i++){
        for(unsigned long j = 0; j < lengthPQ; j++){
            cout << message[i][j] << " ";
            if(message[i][j] == 1){
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