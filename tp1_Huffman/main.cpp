#include <iostream>
#include <fstream>
#include <queue>

#include "Symbol.hpp"
#include "PtrComp.hpp"

using namespace std;


string readFile(string fileName) {
    string source = "";
    ifstream flux(fileName);
    if (flux) {
        string ligne;
        while (getline(flux, source)) {
            source += ligne;
        }
    } else {
        cout << "Error : cannot open this file !";
    }
    return source;
}

void writeFile(string const fileName, string text) {
    ofstream flux(fileName.c_str(), ios::app);
    if (flux) {
        flux << text << endl;
    } else {
        cout << "Error : cannon open this file !";
    }
} 

void huffMan(string source) {
    // Calcules de proba
}


int main(int, char **) {
    // readFile("text.txt");
    priority_queue<Symbol *,
                    vector<Symbol *>, PtrComp > pq ;
    
    return 0;
}