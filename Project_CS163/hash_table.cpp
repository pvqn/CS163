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
		t = (t * 31 + (int)(ch - 'a') + 1) % h1_val;

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
	return false;
}

Hash_Table::Hash_Table() { table.resize(h1_val); }

Hash_Table::Hash_Table(const Hash_Table& other) : table(other.table) {}

Hash_Table& Hash_Table::operator=(const Hash_Table& other)
{
	table = other.table;

	return *this;
}

void Hash_Table::add_to_table(std::string keyword, Word word) {}

void Hash_Table::remove_from_table(std::string keyword, Word word) {}
