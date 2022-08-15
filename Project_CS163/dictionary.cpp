#include "dictionary.h"

#include <fstream>
#include <filesystem>
#include <random>
#include <ctime>

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

	std::ifstream stop;
	stop.open(main_folder + "stopwords.txt");

	if (in.is_open() && stop.is_open())
	{
		std::string line;

		getline(in, line);

		if (in.fail()) return;

		delim = line[0];

		Ternary_Search_Tree stop_tree;

		while (getline(stop, line))
		{
			for (char& ch : line)
				ch = toupper(ch);

			Word eow;
			bool is_valid;
			stop_tree.root = stop_tree.insert_helper(stop_tree.root, line, "1",
				0, nullptr, is_valid, eow);
		}

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

				std::vector<std::string> split = util::str::split(def);

				std::vector<std::string> hashing(split.size(), "");

				for (const std::string& keyword : split)
				{
					if (stop_tree.search_helper(stop_tree.root, keyword, 0))
						continue;

					size_t h = (7 + 31 * (size_t)keyword.front()
						+ 31 * 31 + (size_t)keyword.back()) % hashing.size();

					for (size_t i = 0; i < hashing.size(); i++)
					{
						if (!hashing[(h + i) % hashing.size()].empty())
						{
							if (hashing[(h + i) % hashing.size()] == keyword) break;
						}
						else
						{
							hashing[(h + i) % hashing.size()] = keyword;
							keyword_table.add_to_table_helper(keyword, w, true);
							break;
						}
					}
				}
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
		std::string find;

		word_tree.words_cache.push_back(w);
		std::ifstream stopwords(main_folder + "stopwords.txt");

		if (stopwords)
		{
			std::string tmp;
			while (getline(stopwords, tmp))
			{
				for (char& ch : tmp)
					ch = toupper(ch);

				find += (find.empty() ? "" : " ") + tmp;
			}
		}

		for (const std::string& keyword : util::str::split(def))
		{
			if (find.find(keyword) != std::string::npos)
				keyword_table.add_to_table_helper(keyword, w);
		}
	}
}

void Dictionary::remove(std::string word)
{
	TST_Node* search = word_tree.search_helper(word_tree.root, word, 0);

	if (search)
	{
		Word w = Word(search);

		std::string def = w.get_definition();

		for (const std::string& str : util::str::split(def))
			keyword_table.remove_from_table_helper(str, w);

		bool find = false;

		for (size_t i = 0; i < word_tree.words_cache.size() - 1; i++)
		{
			if (w == word_tree.words_cache[i])
			{
				util::algo::swap(word_tree.words_cache[i], word_tree.words_cache.back());
				find = true;
				break;
			}
		}

		if (find) word_tree.words_cache.pop_back();

		word_tree.remove_helper(word_tree.root, word, 0);
	}
}

void Dictionary::edit_definition(std::string word, std::string def)
{
	TST_Node* node = word_tree.search_helper(word_tree.root, word, 0);

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
