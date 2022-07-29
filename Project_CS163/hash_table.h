#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include <vector>
#include <string>
#include <cstddef>

#include "ternary_seach_tree.h"

class Hash_Table_Bucket
{
private:
	Word word;
	std::string keyword;
	size_t hash = 0;

public:
	Hash_Table_Bucket() = default;

	Hash_Table_Bucket(std::string keyword_, size_t hash_, Word word_);

	bool operator==(const Hash_Table_Bucket& other);

	bool operator<(const Hash_Table_Bucket& other);

	bool operator>(const Hash_Table_Bucket& other);

	friend class Hash_Table;
};

class Hash_Table
{
private:
	const size_t h1_val = 1009;
	const long long h2_val = 1000000097;

	std::vector<std::vector<Hash_Table_Bucket>> table;

	size_t hashing_1(std::string s);

	long long hashing_2(std::string s);

	bool binary_search(std::vector<Hash_Table_Bucket>& v, std::string cmp,
		Word word, size_t& index);

public:
	Hash_Table();

	Hash_Table(const Hash_Table& other);

	~Hash_Table() = default;

	Hash_Table& operator=(const Hash_Table& other);

	// Add a word to the hash_table
	void add_to_table(std::string keyword, Word word);

	// Remove a word from the hash_table
	void remove_from_table(std::string keyword, Word word);

	// TODO: add functions to find by keywords
	std::vector<Word> find_by_keyword(std::string keyword);
};

#endif
