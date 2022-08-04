#ifndef DICTIONARY_H_
#define DICTIONARY_H_
#include <QString>
#include <QStringLiteral>
#include <QChar>

#include "hash_table.h"
#include "ternary_seach_tree.h"

const QString main_folder = "dataset/";

class Dictionary
{
private:
    QString dataset_name;
    QChar delim;

	Hash_Table keyword_table;
	Ternary_Search_Tree word_tree;


public:
    Dictionary(QString dataset);

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

	// Cache the tree into file_name.txt file
	void cache();

	// Adding a word to HIS_ file along with date
    void pushing_into_history_file(QString word);
public:
    void reset();

	// Check if the Dictionary is occupied
    bool dataset_is_equal(QString name);

    QString get_dataset_name();

	// Get the size of the dictionary
	size_t get_dictionary_size();

	// Insert a word to both TST and Hash Table
    void insert(QString word, QString def);

	// Delete word with definition
    void remove(QString word);

	// Delete the definition of a word
    void edit_definition(QString word, QString def);

	// Search a word by definition
    Word search_for_definition(QString word, bool add_to_history);

    std::vector<Word> search_for_definition(QString keyword);

	// Add or remove a word in the favorite list (0 to remove, 1 to add)
    void action_on_favorite_file(QString word, bool status);

	// From FAV_file_name.txt
	std::vector<Word> get_favorite_list();

	// From HIS_file_name.txt
    std::vector<QString> get_history_list();

	// Clear the whole history
	void clear_history();

	// Random n words in the tree
	std::vector<Word> random_words(size_t n);

    std::vector<QString> get_word_prediction(QString prefix);

    std::vector<QString> get_keyword_prediction(QString prefix);
};

#endif
