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


vector<long> integer2Vector(int message){
    vector<long> query;
    while(message){
        if(message & 1){
            query.push_back(1);
        }
        else{
            query.push_back(0);
        }
        message >>= 1;
    }

    return query;
}

vector<long> vector2Vector(vector<long> partialquotient, int lengthPQ){
    vector<long> query;
    for(unsigned long i = 0; i < partialquotient.size(); i++){
        int j = 0;
        while(j < lengthPQ){
            if(partialquotient[i] & 1){
                query.push_back(1);
            }
            else{
                query.push_back(0);
            }
            partialquotient[i] >>= 1;
            j++;
        }
    }

    assert(query.size() == lengthPQ * partialquotient.size());
    for(int i = 0; i < query.size(); i++){
        cout << query[i] << "\t";
    }
    cout << endl;

    return query;
}