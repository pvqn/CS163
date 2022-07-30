#include <iostream>
#include "database.h"
#include "util.h"

int main()
{
	Database base;

	base.change_dataset("people", ',');

	std::cin.get();

	return 0;
}