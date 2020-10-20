#ifndef PTRCOMP
#define PTRCOMP

#include "Symbol.hpp"

class PtrComp {
    public:
        PtrComp();
        bool operator() (const Symbol &, const Symbol &) const;
};



#endif