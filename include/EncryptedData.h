#ifndef ENCRYPTEDDATA_H
#define ENCRYPTEDDATA_H

#include <cstdlib>
#include <vector>

#include "../../../Library/HElib-master/src/EncryptedArray.h"
#include "../../../Library/HElib-master/src/Ctxt.h"

class EncryptedData{
    private:
        Ctxt            numCtxt;        // numerator
        Ctxt            denCtxt;        // denominator
        vector<Ctxt>    cfCtxt;         // its continued fraction
        long            lengthPQ = 0;   // maximum length (bit size) of partial quotients
        long            numPQ = 0;      // number of partial quotients
    
    public:
        EncryptedData();
        ~EncryptedData();

        Ctxt            getNumCtxt()    {return numCtxt;}
        Ctxt            getDenCtxt()    {return denCtxt;}
        vector<Ctxt>    getCFCtxt()     {return cfCtxt;}
        long            getLengthPQ()   {return lengthPQ;}
        long            getNumPQ()      {return numPQ;}
        
        void            SetNumCtxt(Ctxt numCtxt)        {numCtxt    = this -> numCtxt;}
        void            SetDenCtxt(Ctxt denCtxt)        {denCtxt    = this -> denCtxt;}
        void            SetCFCtxt(vector<Ctxt> cfCtxt)  {cfCtxt     = this -> cfCtxt;}
        void            SetLengthPQ(long lengthPQ)      {lengthPQ   = this -> lengthPQ;}
        void            SetNumPQ(long numPQ)            {numPQ      = this -> numPQ;}
};

#endif