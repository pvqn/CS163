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

		template <class T>
		bool binary_search(std::vector<T>& v, T cmp, size_t& index)
		{
			size_t left = 0, right = v.size() ? v.size() - 1 : 0;

			while (left != right)
			{
				auto mid = left + (right - left) / 2;

				if (cmp == v[mid])
				{
					index = mid;
					return true;
				}
				else if (cmp < v[mid])
					right = mid;
				else
					left = mid + 1;
			}

			index = left;
			return false;
		}
	}
	namespace str
	{
		std::vector<std::string> split(std::string definition);
	}
}

#endif
