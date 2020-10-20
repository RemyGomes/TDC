#include <iostream>
#include "Symbol.hpp"


Symbol::Symbol(){
    name = "a";
    freq = 0.0;
    code = "0";
    leaf = false;
    left = nullptr;
    right = nullptr;
}

Symbol::Symbol(std::string n,  double f, std::string c, bool l) {
    name = n;
    freq = f;
    code = c;
    leaf = l;
    left = nullptr;
    right = nullptr;
}

Symbol::Symbol(std::string n, double f, std::string c, bool l, Symbol* le, Symbol* r) {
    name = n;
    freq = f;
    code = c;
    leaf = l;
    left = nullptr;
    right = nullptr;
}

Symbol::Symbol(const Symbol& symbol) {
    name = symbol.name;
    freq = symbol.freq;
    code = symbol.code;
    leaf = symbol.leaf;
    left = symbol.left;
    right = symbol.right;
}
Symbol& Symbol::operator=(const Symbol& symbol) {
    if (&symbol != this) {
        delete left;
        delete right;
        name = symbol.name; 
        freq = symbol.freq; 
        code = symbol.code; 
        leaf = symbol.leaf; 
        left = symbol.left; 
        right = symbol.right; 
    }
    return *this;
} 

Symbol::~Symbol(){
    delete left;
    delete right;
}

bool Symbol::operator<(Symbol const & symbol) const {
    // TODO : change it
    Symbol MySymbol = symbol;
    return true;
}