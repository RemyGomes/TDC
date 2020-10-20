#include "PtrComp.hpp";

#include <iostream>

bool PtrComp::operator() (const Symbol & s1, const Symbol & s2) const {
    return (s1 < s2);
}



