#include "dictionary.h"

#include <fstream>

#include "util.h"

Dictionary::Dictionary(std::string dataset, char deli)
	: dataset_name(dataset), delim(deli) {}

Dictionary::Dictionary(const Dictionary& other)
	: dataset_name(other.dataset_name), delim(other.delim)
{
	keyword_table = other.keyword_table;
	word_tree = other.word_tree;
}

Dictionary::~Dictionary() { cache(); }

void Dictionary::load() 
{
	if (!std::filesystem::exists(main_folder + dataset_name + ".txt"))
		std::filesystem::copy_file(main_folder + "ORG_" + dataset_name + ".txt", 
			main_folder + dataset_name + ".txt");

	std::ifstream in;

	in.open(main_folder + dataset_name + ".txt");

	if (in.is_open())
	{
		std::string line;

		while (getline(in, line))
		{
			std::string word, def;

			size_t i = 0;

			while (i < line.size() && line[i] != delim)
				word.push_back(line[i]);

			if (i >= line.size()) continue;

			def = line.substr(i + 1);

			bool is_valid;

			word_tree.insert(word, def, is_valid);

			if (is_valid)
			{
				Word w = word_tree.search(word);

				for (const std::string& keyword : util::str::split(def))
				{
					keyword_table.add_to_table(keyword, w);
				}
			}
		}
	}
}

void Dictionary::reset()
{
	word_tree.~Ternary_Search_Tree();

	keyword_table.~Hash_Table();

	std::filesystem::copy_file(main_folder + "ORG_" + dataset_name + ".txt", 
		main_folder + dataset_name + ".txt");

	load();
}

void Dictionary::cache()
{
	std::ofstream out;

	out.open(main_folder + dataset_name + ".txt");

	if (out) word_tree.print(delim, out);
}

bool Dictionary::dataset_is_equal(std::string name, char delim)
{
	return (name == dataset_name) && (delim == this->delim);
}

size_t Dictionary::get_dictionary_size()
{
	return word_tree.get_words_list().size();
}

void Dictionary::insert(std::string word, std::string def)
{
	bool is_valid;

	word_tree.insert(word, def, is_valid);

	if (is_valid)
	{
		Word w = word_tree.search(word);

		for (const std::string& keyword : util::str::split(def))
			keyword_table.add_to_table(keyword, w);
	}
}

void Dictionary::remove(std::string word)
{
	Word w = word_tree.search(word);

	if (!w.get_word().empty())
	{
		std::string def = w.get_definition();

		word_tree.remove(word);

		for (const std::string& keyword : util::str::split(def))
			keyword_table.remove_from_table(keyword, w);
	}
}

Word Dictionary::search_for_definition(std::string word)
{
	return word_tree.search(word);
}

std::vector<Word> Dictionary::get_favorite_list() 
{
	std::ifstream in;
	in.open(main_folder + "FAV_" + dataset_name + ".txt");
	return {}; 
}

std::vector<Word> Dictionary::get_history_list() 
{ 
	std::ifstream in;
	in.open(main_folder + "HIS_" + dataset_name + ".txt");

	return {}; 
}

std::vector<Word> Dictionary::random_words(size_t n) { return {}; }

std::vector<std::string> Dictionary::get_prediction(std::string prefix) { return {}; }
