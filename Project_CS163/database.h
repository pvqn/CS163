#ifndef DATABASE_H_
#define DATABASE_H_
#include <vector>

#include "dictionary.h"

/**
 * File management:
 * - The folder contains all the files is dataset/
 * - There are 2 type of files:
 *   + List of dataset (dataset.txt): contains the dataset name (no .txt)
 *     and delimeter
 *   + Dataset-related files (file_name): there are 4 files in total
 *       ~ ORG_file_name.txt: original file
 *       ~ file_name.txt: main file contains all the words
 *       ~ FAV_file_name.txt: favorite list
 *       ~ HIS_file_name.txt: history of each dataset
 * 
 * Format of the HIS_ file: word!dd!mm!yyyy (! be de delimiter of each file)
 * ex: emotional.txt use "\t" as delimiter, so the format is: CS163\t30\t07\t2022
 *     slang.txt use "`" as delimiter, so the format is: CS163`30`07`2022
 *     english.txt use "," as delimiter, so the format is: CS163,30,07,2002
 * 
 * (OPTIONAL): convert all files to binary (except ORG_file_name.txt)
 *             -> is this case, use std::string.c_str() to have '\0'
 **/

class Database
{
private:
	std::vector<Dictionary*> list{ nullptr };
	Dictionary* current = nullptr;

	// cache_mode: safe old tree in RAM until the program is teminated
	bool cache_mode = false;
public:
	Database() = default;
	~Database();

	void set_cache_mode(bool to_cache);

	void change_dataset(std::string file_name);

	Dictionary& get();
};

#endif
