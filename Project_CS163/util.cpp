#include "util.h"

std::vector<std::string> util::str::split(std::string definition)
{
	std::vector<std::string> words;

	std::string temp;

	for (char ch : definition)
	{
		bool check = (65 <= ch && ch <= 90) || (97 <= ch && ch <= 122);

		if (check)
			temp.push_back(toupper(ch));
		else
		{
			if (!temp.empty())
				words.push_back(temp);
			temp.clear();
		}
	}

	if (!temp.empty())
		words.push_back(temp);

	return words;
}