#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>

namespace util
{
	namespace str
	{
		std::vector<std::string> split(std::string str);
	}

	namespace algo
	{
		template <typename T>
		void swap(T& a, T& b)
		{
			T temp = a;
			a = b;
			b = temp;
		}

		template <typename T>
		size_t partition(std::vector<T>& v, size_t begin, size_t end)
		{
			T pivot = v[end];
			size_t left = begin, right = end - 1;

			while (true)
			{
				while (left <= right && v[left] < pivot) ++left;
				while (left <= right && v[right] > pivot) --right;

				if (right <= left) break;

				swap(v[left], v[right]);
				++left; ++right;
			}

			swap(v[left], v[end]);
			return left;
		}

		template <typename T>
		void sort_recursive(std::vector<T>& v, size_t begin, size_t end)
		{
			if (begin < end)
			{
				size_t index = partition(v, begin, end);

				sort_recursive(v, begin, index - 1);
				sort_recursive(v, index + 1, end);
			}
		}

		template <typename T>
		void full_vector_sort(std::vector<T>& v)
		{
			sort_recursive(v, 0, v.size() - 1);
		}
	}
}

#endif // !UTIL_HPP_
