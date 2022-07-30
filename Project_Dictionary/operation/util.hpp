#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <cmath>
#include <string>
#include <vector>

namespace util
{
    namespace str
    {
        std::string to_upper(std::string input);

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
    }
}

#endif // !UTIL_HPP_
