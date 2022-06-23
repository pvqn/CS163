#include "string_util.hpp"
#include "TernarySearchTree.hpp"
void util::insertStopWord(TernarySearchTree& stopword)
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
std::vector<std::string> util::split(std::string str)
{
	std::vector<std::string> result;
	//lowercase
	for (int i = 0; i < str.size(); ++i) str[i] = tolower(str[i]);
	TernarySearchTree stopword;
	insertStopWord(stopword);
	int i = 0;
	while (true)
	{
		std::string temp;
		while (i < str.size() && isalpha(str[i]))
		{
			temp.push_back(str[i]);
			++i;
		}
		++i;
		if (!stopword.search(temp)) result.push_back(temp);
		if (i >= str.size()) break;
	}
	return result;
}
