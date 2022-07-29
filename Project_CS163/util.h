#ifndef UTIL_H_
#define UTIL_H_
#include <string>
#include <vector>
#include <random>

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

		template <class T>
		bool binary_search(std::vector<T>& v, T cmp, size_t& index)
		{
			auto left = v.begin(), right = v.end() - 1;

			auto mid = left + (right - left) / 2;

			while (left != right)
			{
				if (cmp == *mid)
				{
					index = mid - v.begin();
					return true;
				}
				else if (cmp < *mid)
					right = mid - 1;
				else
					left = mid + 1;

				mid = left + (right - left) / 2;
			}

			index = left + (cmp > *left ? 1 : 0) - v.begin();
			return false;
		}
	}
	namespace str
	{
		std::vector<std::string> split(std::string definition);
	}
}

#endif
