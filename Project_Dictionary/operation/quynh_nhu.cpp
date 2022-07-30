
#include "dictionary.hpp"

void printOutHistory()
{
    std::ifstream fin;
    fin.open("History.txt");
    //std::cout<<"History of words: \n";
    while (!fin.eof())
    {
        std::string t; fin >> t;
        //std::cout << t << "\n";
    }
}
std::string getWord(TreeNode* eow)
{
    std::string word="";
    while (eow)
    {
        if (eow->parent->mid == eow) word.push_back(eow->data);
        eow = eow->parent;
    }
    std::reverse(word.begin(), word.end());
    return word;
}

