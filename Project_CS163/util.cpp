#include "util.h"

std::vector<std::string> util::str::split(std::string definition)
{
	std::vector<std::string> words;

	const char stop[] = { ':', '\t', '\'', ' ',
						',', '.', '?', '!',
						'@', '#', '(', ')',
						'|' , ';', '"', '`', '\'',
						'!', '$', '%', '^', '&', '*',
						'-', '_', '+', '=', '{', '}',
						'\\', '<', '>' };

	std::string temp;

	for (char ch : definition)
	{
		bool check = true;

		for (char st : stop)
			check = (ch != st);

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