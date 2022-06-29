
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
	return word;
}
void randomWord4Def(std::vector<std::string> words)
{
	srand(time(0));
	int index = rand() % 4;
	// std::cout 4 definitions
	// std::cout getword(word[index]
}
