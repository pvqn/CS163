#include <fstream>

#include "TernarySearchTree.hpp"

void insertStopWord(TernarySearchTree& stopword)
{
    std::ifstream fin;
    fin.open("stop_word.txt");
    std::string temp;
    while (!fin.eof())
    {
        fin >> temp;
        stopword.insert(temp, "1");
    }
    fin.close();
}
