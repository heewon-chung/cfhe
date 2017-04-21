#ifndef ENCRYPTEDDATABASE_H
#define ENCRYPTEDDATABASE_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

#include "EncryptedData.h"

class EncryptedDB{
    private:
        vector<EncryptedData>   encData;        // encrypted data
        long                    dbSize = 0;     // number of encrypted data
    
    public:
        EncryptedDB();
        ~EncryptedDB();

        vector<EncryptedData>   getEncData()    {return encData;}
        long                    getDBSize()     {return dbSize;}
};

#endif