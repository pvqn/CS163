#ifndef dictionary_hpp
#define dictionary_hpp

#include <cstdio>
#include "TernarySearchTree.hpp"
#include <fstream>

class Dictionary {
private:
    int size;
    
    TernarySearchTree data;
    
    std::string pathCurrentDataset;
    
public:
    Dictionary() {
        size = 0;
        pathCurrentDataset = "slang.txt"; //might change later
        load(pathCurrentDataset);
    }
    
    void load(std::string path) {
        pathCurrentDataset = ((path.substr(0,4) == "ORG_") ? path.substr(4, path.length()-3) : path);
        std::ifstream fin(path);
        std::string line;
        while (getline(fin, line)) {
            unsigned long delim = line.find('`');
            data.insert(line.substr(0, delim), line.substr(delim+1, line.length()-delim-1));
            ++size;
        }
        fin.close();
    }
    
    void reset() {
        data.~TernarySearchTree();
        size = 0;
        load("ORG_" + pathCurrentDataset);
    }
};

void actionOnFavList(std::string a, bool status);
void printOutHistory();
void randomWord4Def(Hash_Table def);
#endif /* dictionary_hpp */
