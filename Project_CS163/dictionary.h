#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <string>

#include "hash_table.h"
#include "ternary_seach_tree.h"

const std::string main_folder = "dataset/";

class Dictionary
{
private:
	const std::string dataset_name;
	const char delim;

	Hash_Table keyword_table;
	Ternary_Search_Tree word_tree;


public:
	Dictionary(std::string dataset, char deli);

	Dictionary(const Dictionary& other);

	~Dictionary();

private:
	/**
	 * Load data from file_name.txt
	 *
	 * Remember to check if the file_name.txt exist, else we have to copy
	 * the data from ORG_file_name.txt to file_name.txt first
	 **/
	void load();

	/**
	 * Reset the data to the ORG_file_name.txt state
	 **/
	void reset();

	// Cache the tree into file_name.txt file
	void cache();

public:
	// Check if the Dictionary is occupied
	bool dataset_is_equal(std::string name, char delim);

	// Get the size of the dictionary
	size_t get_dictionary_size();

	// Insert a word to both TST and Hash Table
	void insert(std::string word, std::string def);

	// Delete word with definition
	void remove(std::string word);

	// Search a word by definition
	Word search_for_definition(std::string word);

	// From FAV_file_name.txt
	std::vector<Word> get_favorite_list();

	// From HIS_file_name.txt
	std::vector<std::string> get_history_list();

	// Random n words in the tree
	std::vector<Word> random_words(size_t n);

	std::vector<std::string> get_prediction(std::string prefix); 
	
};

#endif
