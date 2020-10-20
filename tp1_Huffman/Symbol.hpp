#ifndef SYMBOL
#define SYMBOL

#include <string>

/**
 * @class Symbol
 * 
 */

class Symbol {
    public: 
        std::string name;
        double freq;
        std::string code;
        bool leaf;
        Symbol *left, *right;

        // Constructors
        Symbol();
        Symbol(std::string,  double, std::string, bool);
        Symbol(std::string, double, std::string, bool, Symbol*, Symbol*);
        Symbol(const Symbol&); // Clone
        Symbol& operator=(const Symbol&); 

        // Destructors
        ~Symbol();

        bool operator<(Symbol const &) const;

        // Getters & Setters

        // Getters
        inline std::string getName() {
            return name;
        }
        inline double getFreq() {
            return freq;
        }
        inline std::string getCode() {
            return code;
        }
        inline bool isLeaf() {
            return leaf;
        }
        inline Symbol* getLeft() {
            return left;
        }
        inline Symbol* getRight() {
            return right;
        }

        // Setters
        inline void setName(std::string n) {
            name = n;
        }
        inline void setFreq(double f) {
            freq = f;
        }
        inline void setCode(std::string c) {
            code = c;
        }
        inline void setLeaf(bool l) {
            leaf = l;
        }
        inline void setLeft(Symbol* l) {
            left = l;
        }
        inline void setRight(Symbol* r) {
            right = r;
        }
};

#endif