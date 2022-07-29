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
		std::vector<std::string> split(std::string definition)
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
	}
}

#endif
