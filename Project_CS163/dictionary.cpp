#include "dictionary.h"

Dictionary::Dictionary(std::string dataset, char deli)
	: dataset_name(dataset), delim(deli) {}

Dictionary::Dictionary(const Dictionary& other)
	: dataset_name(other.dataset_name), delim(other.delim)
{
	keyword_table = other.keyword_table;
	word_tree = other.word_tree;
}

void Dictionary::load() {}

void Dictionary::reset() {}

void Dictionary::cache() {}

void Dictionary::insert(std::string word, std::string def) {}

void Dictionary::remove(std::string word) {}

Word Dictionary::search_for_definition(std::string word) { return Word(); }

std::vector<Word> Dictionary::get_favorite_list() { return {}; }

std::vector<Word> Dictionary::get_history_list() { return {}; }

std::vector<Word> Dictionary::random_n_words(size_t n) { return {}; }

std::vector<std::string> Dictionary::get_prediction(std::string prefix) { return {}; }
