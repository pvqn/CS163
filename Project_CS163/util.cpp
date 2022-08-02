#include "util.h"

#include <locale>

std::vector<std::string> util::str::split(std::string definition)
{
	std::vector<std::string> words;

	std::locale loc;
	std::string temp;

	for (unsigned char ch : definition)
	{
		if (std::isalnum(ch, loc))
			temp.push_back(std::toupper(ch, loc));
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