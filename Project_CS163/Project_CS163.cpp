#include <iostream>
#include "main_dictionary.hpp"

int main()
{
	Dictionary test("Viet-Anh", '`');
	test.print_dic();
	test.random4Word();
	return 0;
}
