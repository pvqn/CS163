#ifndef UTIL_H_
#define UTIL_H_
#include <string>
#include <vector>
#include <memory>

namespace util
{
	namespace algo
	{
		template <class T>
		void swap(T& a, T& b)
		{
			T temp = std::move(a);
			a = std::move(b);
			b = std::move(temp);
		}
	}
	namespace str
	{
		std::vector<std::string> split(std::string definition);
	}
}

#endif
