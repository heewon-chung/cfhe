#include <cstdlib>
#include <vector>

#include "generalTools.h"

using namespace std;

int main(void){

    int lengthPQ = 4;
    vector<long> pq;

    pq.push_back(5);
    pq.push_back(4);
    pq.push_back(10);
    pq.push_back(15);
    
    vector<long> query = vector2Vector(pq, 4);   

    return 0;
}