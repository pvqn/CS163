#include "dictionary.h"

#include <fstream>
#include <filesystem>

#include "util.h"

Dictionary::Dictionary(std::string dataset, char deli)
	: dataset_name(dataset), delim(deli) 
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

		while (getline(in, line))
		{
			std::string word, def;

			size_t i = 0;

			while (i < line.size() && line[i] != delim)
				word.push_back(line[i++]);

			if (i >= line.size())
				continue;

			def = line.substr(i + 1);

			bool is_valid;

			Word w;

			word_tree.insert(word, def, is_valid, w);

			if (is_valid)
			{
				for (std::string keyword : util::str::split(def))
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

	if (out)
		word_tree.print(delim, out);
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

	Word w;

	word_tree.insert(word, def, is_valid, w);

	if (is_valid)
	{
		for (const std::string& keyword : util::str::split(def))
			keyword_table.add_to_table(keyword, w);
	}
}

void Dictionary::remove(std::string word)
{
	Word w = Word();

	word_tree.remove(word, w);

	if (!w.get_word().empty())
	{
		std::string def = w.get_definition();

		for (std::string keyword : util::str::split(def))
			keyword_table.remove_from_table(keyword, w);
	}

}

Word Dictionary::search_for_definition(std::string word)
{
	std::ofstream out;
	out.open(main_folder + "HIS_" + dataset_name + ".txt", std::ios::app);
	out << word <<"\n"; 
	return word_tree.search(word);
}

std::vector<Word> Dictionary::get_favorite_list()
{
	std::ifstream in;
	in.open(main_folder + "FAV_" + dataset_name + ".txt");
	return {};
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
	return history;
}

std::vector<Word> Dictionary::random_words(size_t n) 
{ 
	std::vector<Word> randomWords;
	srand((unsigned int)time(0));
	std::vector<int> index;

	//random 4 number
	while (index.size() < 4)
	{
		int t = rand() % n;
		bool check = false;
		for (int i = 0; i < index.size(); ++i)
			if (t == index[i])
			{
				check = true;
				break;
			}
		if (!check) index.push_back(t);
	}

	//sort 4 number
	for (int i = 0; i < index.size(); ++i)
		for (int j = i; j < index.size(); ++j)
			if (index[i] > index[j])
			{
				int temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			}

	std::vector<Word> temp = word_tree.get_words_list();
	for (int i = 0; i < 4; ++i)
		randomWords.push_back(temp[index[i]]);
	return randomWords;
}

std::vector<std::string> Dictionary::get_prediction(std::string prefix) { return {}; }

void Dictionary::action_on_favorite_file(std::string word, bool status)
{
	std::string path = main_folder + "FAV_" + dataset_name + ".txt"; 
	std::fstream fst(path, std::ios::app); fst.close();
	std::fstream fst2("temp.txt", std::ios::app);
	fst.open(path, std::ios::in);
	std::string x; //temp string
	bool duplicated = false;
	while (getline(fst, x)) {
		if (word == x) {
			duplicated = true;
			if (!status) fst2 << x << "\n";
		}
		else fst2 << x << "\n";
	}
	fst.close();
	if (status == 0 && !duplicated) fst2 << word << "\n";
	fst2.close();
	const char* p = path.c_str();
	remove(p);
	rename("temp.txt", p);
}