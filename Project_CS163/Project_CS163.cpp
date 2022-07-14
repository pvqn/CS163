#include <iostream>
#include "TernarySearchTree.hpp"
#include "hash_table.hpp"
#include "dictionary.hpp"
#include "main_dictionary.hpp"

int main()
{
	Dictionary dDic;
	//dDic.print_dic();
	//std::string word = "babe";
	//dDic.update(word, "beto");
	//dDic.insert(word, "beto");
	//TreeNode* position = dDic.search(word);
	//std::cout << position->data << '\n';
	//if (position) std::cout << position->def << '\n'; else std::cout << "NOOOO" << '\n';
	//dDic.reset();
	dDic.cache();
	return 0;
}
