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
		std::vector<std::string> split(std::string definition);
	}
}

#endif
