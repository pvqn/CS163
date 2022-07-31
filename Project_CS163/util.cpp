#include <cctype>

#include "util.h"

std::vector<std::string> util::str::split(std::string definition)
{
	std::vector<std::string> words;

	std::string temp;

	for (char ch : definition)
	{
		std::string valid = "`-=[]\;',./~!@#$%^&*()_+{}|:\"<>?";

		if (valid.find(ch) == std::string::npos)
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