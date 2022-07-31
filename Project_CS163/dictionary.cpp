#include "dictionary.h"

#include <fstream>
#include <filesystem>
#include <random>
#include <ctime>
#include <iostream>

#include "util.h"

Dictionary::Dictionary(std::string dataset)
	: dataset_name(dataset)
{
	load();
}

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

		getline(in, line);

		if (in.fail()) return;

		delim = line[0];

		while (getline(in, line))
		{
			size_t i = 0;

			while (i < line.size() && line[i] != delim)
				i++;

			if (i >= line.size())
				continue;

			std::string word = line.substr(0, i), def = line.substr(i + 1);

			bool is_valid = false;

			Word w;

			word_tree.root = word_tree.insert_helper(word_tree.root, word, def, 0, nullptr, is_valid, w);

			if (is_valid)
			{
				word_tree.words_cache.push_back(w);
				for (const std::string& keyword : util::str::split(def))
					keyword_table.add_to_table_helper(keyword, w);
			}
		}
	}
}

void Dictionary::reset()
{
	word_tree.~Ternary_Search_Tree();

	keyword_table.~Hash_Table();
	
	// reset history.txt and fav.txt
	if (std::filesystem::exists(main_folder + "HIS_" + dataset_name + ".txt"))
		std::filesystem::remove(main_folder + "HIS_" + dataset_name + ".txt");
	if (std::filesystem::exists(main_folder + "FAV_" + dataset_name + ".txt"))
		std::filesystem::remove(main_folder + "FAV_" + dataset_name + ".txt");

	std::filesystem::copy_file(main_folder + "ORG_" + dataset_name + ".txt",
		main_folder + dataset_name + ".txt");

	load();
}

void Dictionary::cache()
{
	std::ofstream out;

	out.open(main_folder + dataset_name + ".txt");

	if (out)
	{
		out << delim << '\n';
		word_tree.print_helper(word_tree.root, delim, out);
	}
	out.close();
}

void Dictionary::action_on_favorite_file(std::string word, bool status)
{
	std::string path = main_folder + "FAV_" + dataset_name + ".txt";
	std::string temp_file = main_folder + "temp.txt";

	std::ifstream original(path);
	std::ofstream temp(temp_file);

	if (original && temp)
	{
		std::string str;
		bool duplicate = false;

		while (getline(original, str))
		{
			if (word == str)
			{
				duplicate = true;
				if (!status) temp << str << '\n';
			}
			else
			{
				temp << str << '\n';
			}
		}

		if (status && !duplicate) temp << str << '\n';

		original.close();
		temp.close();

		std::filesystem::copy_file(temp_file, path);

		std::filesystem::remove(temp_file);
	}
}

void Dictionary::pushing_into_history_file(std::string word)
{
	time_t t = time(0);

	struct tm now;
	localtime_s(&now, &t);

	std::ofstream out;
	out.open(main_folder + "HIS_" + dataset_name + ".txt", std::ios::app);

	if (out)
	{
		out << now.tm_year + 1900 << "~"
			<< now.tm_mon + 1 << "~"
			<< now.tm_mday << "~"
			<< word << "\n";
	}
}

bool Dictionary::dataset_is_equal(std::string name)
{
	return name == dataset_name;
}

size_t Dictionary::get_dictionary_size()
{
	return word_tree.words_cache.size();
}

void Dictionary::insert(std::string word, std::string def)
{
	bool is_valid = false;

	Word w;

	word_tree.root = word_tree.insert_helper(word_tree.root, word, def, 0, nullptr, is_valid, w);

	if (is_valid)
	{
		/*size_t index;

		util::algo::binary_search(word_tree.words_cache, w, index);

		if (word_tree.words_cache.empty())
			word_tree.words_cache.push_back(w);
		else
			word_tree.words_cache.insert(word_tree.words_cache.begin() + index + 
				(word_tree.words_cache[index] < w), w);*/

		//insert to wordscache
		word_tree.words_cache.push_back(w);
		
		for (const std::string& keyword : util::str::split(def))
			keyword_table.add_to_table_helper(keyword, w);
	}
}

void Dictionary::remove(std::string word)
{
	std::string def;

	if (word_tree.remove_helper(word_tree.root, word, 0, def))
	{
		Ternary_Search_Tree temp;
		Word w;
		bool dummy;

		// remove in wordscache

		bool check = false;
		for (int i = 0; i < word_tree.words_cache.size()-1; ++i)
		{
			if (word_tree.words_cache[i].get_word() == word)
			{
				check = true;
			}
			if (check)
			{
				word_tree.words_cache[i] = word_tree.words_cache[i + 1];
			}
		}
		word_tree.words_cache.pop_back();

		temp.root = temp.insert_helper(temp.root, word, def, 0, nullptr, dummy, w);

		for (const std::string& word : util::str::split(def))
		{
			keyword_table.remove_from_table_helper(word, w);
		}
	}
}

void Dictionary::edit_definition(std::string word, std::string def)
{
	TST_Node *node = word_tree.search_helper(word_tree.root, word, 0);

	Word w = Word(node);

	if (w.get_word().empty() || w.get_definition() == def) return;

	for (const std::string& keyword_old : util::str::split(w.get_definition()))
		keyword_table.remove_from_table_helper(keyword_old, w);

	for (const std::string& keyword_new : util::str::split(def))
		keyword_table.remove_from_table_helper(keyword_new, w);

	word_tree.update_def_helper(node, def);
}

Word Dictionary::search_for_definition(std::string word, bool add_to_history)
{
	if (add_to_history) pushing_into_history_file(word);

	return Word(word_tree.search_helper(word_tree.root, word, 0));
}

std::vector<Word> Dictionary::search_for_definition(std::string keyword)
{
	return keyword_table.find_by_keyword(keyword);
}

std::vector<Word> Dictionary::get_favorite_list()
{
	std::ifstream in;
	in.open(main_folder + "FAV_" + dataset_name + ".txt");
	std::string t;
	std::vector<Word> fav_list;
	while (std::getline(in, t))
	{
		Word temp = Word(word_tree.search_helper(word_tree.root, t, 0));
		if (!temp.get_word().empty()) fav_list.push_back(temp);
	}
	in.close();
	return fav_list;
}

std::vector<std::string> Dictionary::get_history_list()
{
	std::ifstream in;
	in.open(main_folder + "HIS_" + dataset_name + ".txt");
	std::string t;
	std::vector <std::string> history;
	while (std::getline(in, t))
	{
		history.insert(history.begin(), t);
	}
	in.close();
	return history;
}

void Dictionary::clear_history()
{
	if (std::filesystem::exists(main_folder + "HIS_" + dataset_name + ".txt"))
		std::filesystem::remove(main_folder + "HIS_" + dataset_name + ".txt");
}

std::vector<Word> Dictionary::random_words(size_t n)
{
	std::vector<Word> words = word_tree.words_cache;

	if (words.size() < n) return {};

	std::vector<bool> existed;

	existed.resize(words.size());

	for (size_t i = 0; i < existed.size(); i++)
		existed[i] = 0;

	std::vector<Word> randomWords;

	std::mt19937 md(std::random_device{}());

	std::uniform_int_distribution<size_t> dis(0, words.size() - 1);

	size_t count = 0;

	while (count < n)
	{
		size_t i = dis(md);

		if (existed[i] == 0)
		{
			existed[i] = 1;
			randomWords.push_back(words[i]);
			count++;
		}
	}

	return randomWords;
}

std::vector<std::string> Dictionary::get_prediction(std::string prefix) 
{
	return word_tree.get_prediction_helper(prefix);
}
