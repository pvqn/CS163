#include <iostream>
#include <vector>
#include <chrono>
#include "database.h"

int main()
{
	Database base;

	auto begin = std::chrono::system_clock::now();

	base.change_dataset("emotional");

	auto end = std::chrono::system_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

	std::cout << "DONE! " << d.count() << " seconds\n";

	return 0;
}