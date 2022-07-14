#include <iostream>
#include "TernarySearchTree.hpp"
#include "hash_table.hpp"
#include "dictionary.hpp"
#include "main_dictionary.hpp"

int main()
{
	Dictionary dDic;
	dDic.remove("apple");
	dDic.remove("baby");
	dDic.reset();
	dDic.cache();
	return 0;
}
