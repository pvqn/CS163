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
 * 		 ~ file_name.txt: main file contains all the words
 * 		 ~ FAV_file_name.txt: favorite list
 * 		 ~ HIS_file_name.txt: history of each dataset
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

	void change_dataset(std::string file_name, char delim);

	Dictionary& get();
};

#endif
