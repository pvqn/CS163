#include "hash_table.h"

Hash_Table_Bucket::Hash_Table_Bucket(std::string keyword_, size_t hash_, Word word_)
	: keyword(std::move(keyword_)), hash(hash_), word(word_) {}

bool Hash_Table_Bucket::operator==(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return false;

	if (keyword.size() != other.keyword.size())
		return false;

	if (keyword != other.keyword)
		return false;

	return word == other.word;
}

bool Hash_Table_Bucket::operator<(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return hash < other.hash;

	if (keyword.size() != other.keyword.size())
		return keyword.size() < other.keyword.size();

	if (keyword != other.keyword)
		return keyword < other.keyword;

	return word < other.word;
}

bool Hash_Table_Bucket::operator>(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return hash > other.hash;

	if (keyword.size() != other.keyword.size())
		return keyword.size() > other.keyword.size();

	if (keyword != other.keyword)
		return keyword > other.keyword;

	return word > other.word;
}

size_t Hash_Table::hashing_1(std::string s)
{
	size_t t = 1;

	for (char& ch : s)
		t = (t * 31 + ((size_t)ch - (size_t)'a') + 1) % h1_val;

	return t;
}

long long Hash_Table::hashing_2(std::string s)
{
	long long t = 1;

	for (char& ch : s)
	{
		t = (t * 311 + ((long long)ch - (long long)'a') + 1) % h2_val;
	}

	return t;
}

bool Hash_Table::binary_search(std::vector<Hash_Table_Bucket>& v, std::string cmp,
	Word word, size_t& index)
{
	Hash_Table_Bucket bucket = Hash_Table_Bucket(cmp, hashing_2(cmp), word);

	auto left = v.begin(), right = v.end() - 1;

	auto mid = left + (right - left) / 2;

	while (left != right)
	{
		if (bucket == *mid)
		{
			index = mid - v.begin();
			return true;
		}
		else if (bucket < *mid)
			right = mid - 1;
		else
			left = mid + 1;

		mid = left + (right - left) / 2;
	}

	index = left + (bucket > *left ? 1 : 0) - v.begin();
	return false;
}

Hash_Table::Hash_Table() { table.resize(h1_val); }

Hash_Table::Hash_Table(const Hash_Table& other) : table(other.table) {}

Hash_Table& Hash_Table::operator=(const Hash_Table& other)
{
	table = other.table;

	return *this;
}

void Hash_Table::add_to_table(std::string keyword, Word word)
{
	size_t h1 = hashing_1(keyword);
	size_t index = 0;

	if (!binary_search(table[h1], keyword, word, index))
		table[h1].insert(table[h1].begin() + index,
			Hash_Table_Bucket(keyword, hashing_2(keyword), word));
}

void Hash_Table::remove_from_table(std::string keyword, Word word)
{
	size_t h1 = hashing_1(keyword);
	size_t index = 0;

	if (binary_search(table[h1], keyword, word, index))
		table[h1].erase(table[h1].begin() + index);
}

std::vector<Word> Hash_Table::find_by_keyword(std::string keyword)
{
	std::vector<Word> words;

	size_t h1 = hashing_1(keyword);

	auto begin = table[h1].begin(), end = table[h1].end();

	auto mid = begin + (end - begin) / 2;

	while (begin != end)
	{
		size_t mid_index = mid - table[h1].begin();

		if (keyword == mid->keyword)
			break;
		else if (keyword < mid->keyword)
			end = mid;
		else
			begin = mid;

		mid = begin + (end - begin) / 2;
	}

	if (mid->keyword == keyword)
	{
		size_t mid_index = mid - table[h1].begin();

		for (size_t i = mid_index + 1; i < table[h1].size(); i++)
		{
			if (table[h1][i].keyword != keyword)
				break;

			words.push_back(table[h1][i].word);
		}

		while (true)
		{
			if (table[h1][mid_index].keyword != keyword)
				break;

			words.insert(words.begin(), table[h1][mid_index].word);

			if (mid_index == 0)
				break;
			else
				mid_index--;
		}
	}

	return words;
}
