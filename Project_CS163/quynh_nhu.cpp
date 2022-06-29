
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
