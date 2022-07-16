#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <cmath>
#include <string>
#include <vector>

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
			size_t mid = (end - begin + 1) / 2 + begin;
			size_t median;

			if ((v[begin] > v[end]) ^ (v[begin] > v[mid]))
				median = begin;
			else if ((v[mid] < v[begin]) ^ (v[mid] < v[end]))
				median = mid;
			else
				median = end;

			if (median != end) swap(v[median], v[end]);

			T pivot = v[end];

			size_t left = begin;

			for (size_t i = begin; i < end - 1; ++i)
			{
				if (v[i] < pivot)
					swap(v[i], v[left++]);
			}

			swap(v[left], v[end]);

			return left;
		}

		template <typename T>
		void heapify(std::vector<T>& v, size_t begin, size_t end, size_t index)
		{
			size_t bigger = index;

			size_t left = begin + (index - begin) * 2 + 1;
			size_t right = begin + (index - begin) * 2 + 2;

			if (left <= end && v[left] > v[bigger])
				bigger = left;

			if (right <= end && v[right] > v[bigger])
				bigger = right;

			if (bigger != index)
			{
				swap(v[index], v[bigger]);
				heapify(v, begin, end, bigger);
			}
		}

		template <typename T>
		void heap_sort(std::vector<T>& v, size_t begin, size_t end)
		{
			for (size_t i = begin + (end - begin + 1) / 2 - 1; i > begin; --i)
			{
				heapify(v, begin, end, i);
			}

			heapify(v, begin, end, begin);

			for (size_t i = end - 1; i > begin; --i)
			{
				swap(v[begin], v[i + 1]);
				heapify(v, begin, i, begin);
			}

			swap(v[begin], v[begin + 1]);
		}

		template <typename T>
		void insertion_sort(std::vector<T>& v, size_t begin, size_t end)
		{
			for (size_t i = begin + 1; i <= end; ++i)
			{
				T k = v[i];
				size_t j = i - 1;
				bool neg = false;

				while (!neg && j >= 0 && v[j] > k)
				{
					v[j + 1] = v[j];

					if (j == 0) neg = true;
					else --j;
				}

				v[neg ? 0 : j + 1] = k;
			}
		}

		template <typename T>
		void sort_recursive(std::vector<T>& v, size_t begin, size_t end, size_t depth)
		{
			size_t length = end - begin + 1;

			if (length < 16)
			{
				insertion_sort(v, begin, end);
			}
			else if (depth == 0)
			{
				heap_sort(v, begin, end);
			}
			else if (begin <= end)
			{
				size_t index = partition(v, begin, end);

				if (index > 0) sort_recursive(v, begin, index - 1, depth - 1);
				sort_recursive(v, index + 1, end, depth - 1);
			}
		}

		template <typename T>
		void sort(std::vector<T>& v)
		{
			size_t depth = static_cast<size_t>(2 * log2(v.size() - 1));
			sort_recursive(v, 0, v.size() - 1, depth);
		}

		template <typename T>
		void sort(std::vector<T>& v, size_t begin, size_t end)
		{
			size_t depth = static_cast<size_t>(2 * log2(end - begin + 1));
			sort_recursive(v, begin, end, depth);
		}
	}
}

#endif // !UTIL_HPP_
