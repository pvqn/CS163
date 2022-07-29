#ifndef UTIL_H_
#define UTIL_H_
#include <string>
#include <vector>

namespace util
{
	namespace algo
	{
		template <class T>
		void swap(T& a, T& b)
		{
			T temp = a;
			a = b;
			b = temp;
		}
	}
	namespace str
	{
		std::string to_upper(std::string input)
		{
			for (char& c : input)
				c = toupper(c);
			return input;
		}
		std::vector<std::string> split(std::string definition)
		{
			std::vector<std::string> words;

			std::vector<char> stop = { ':', '\t', '\'', ' ',
										',', '.', '?', '!',
										'@', '#', '(', ')',
										'|' , ';', '"', '`', '\'',
										'!', '$', '%', '^', '&', '*',
										'-', '_', '+', '=', '{', '}',
										'\\', '<', '>' };

			std::string temp;

			for (size_t i = 0; i < definition.size(); i++)
			{
				bool check = true;

				for (int j = 0; j < stop.size() && check; j++)
					check = (definition[i] != stop[j]);

				if (check)
				{
					temp.push_back(definition[i]);
				}
				else
				{
					if (!temp.empty())
						words.push_back(to_upper(temp));
					temp.clear();
				}

			}

			if (!temp.empty())
				words.push_back(util::str::to_upper(temp));

			return words;
		}
	}
}

#endif
