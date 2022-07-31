#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <string>

#include "hash_table.h"
#include "ternary_seach_tree.h"

const std::string main_folder = "dataset/";

class Dictionary
{
private:
	std::string dataset_name;
	char delim;

	Hash_Table keyword_table;
	Ternary_Search_Tree word_tree;


public:
	Dictionary(std::string dataset);

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

	// Adding a word to HIS_ file along with date
	void pushing_into_history_file(std::string word);
public:
	// Check if the Dictionary is occupied
	bool dataset_is_equal(std::string name);

	// Get the size of the dictionary
	size_t get_dictionary_size();

	// Insert a word to both TST and Hash Table
	void insert(std::string word, std::string def);

	// Delete word with definition
	void remove(std::string word);

	// Delete the definition of a word
	void edit_definition(std::string word, std::string def);

	// Search a word by definition
	Word search_for_definition(std::string word, bool add_to_history);

	std::vector<Word> search_for_definition(std::string keyword);

	// Add or remove a word in the favorite list (0 to remove, 1 to add)
	void action_on_favorite_file(std::string word, bool status);

	// From FAV_file_name.txt
	std::vector<Word> get_favorite_list();

	// From HIS_file_name.txt
	std::vector<std::string> get_history_list();

	// Clear the whole history
	void clear_history();

	// Random n words in the tree
	std::vector<Word> random_words(size_t n);

	std::vector<std::string> get_prediction(std::string prefix);
};

#endif
