#include <iostream>
#include "main_dictionary.hpp"

int main()
{
	Dictionaries* database = new Dictionaries;

	database->switch_dataset("people", '`');

	std::cin.get();

	delete database;
	return 0;
}
