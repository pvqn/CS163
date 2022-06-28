#include "util.hpp"
#include "TernarySearchTree.hpp"

std::vector<std::string> util::str::split(std::string str)
{
	std::vector<std::string> result;
	//lowercase
	for (int i = 0; i < str.size(); ++i) str[i] = tolower(str[i]);
	int i = 0;

	TernarySearchTree stopword;
	insertStopWord(stopword);

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
