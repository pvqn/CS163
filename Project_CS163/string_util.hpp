#ifndef STRING_UTIL_HPP_
#define STRING_UTIL_HPP_
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include "TernarySearchTree.hpp"
namespace util
{
	/**
	 * @brief Split the string into words
	 * 
	 * @param str The original string
	 * @return Vector of words 
	 */
	std::vector<std::string> split(std::string str);
	void insertStopWord(TernarySearchTree& stopword);

};

#endif // !STRING_UTIL_HPP_
