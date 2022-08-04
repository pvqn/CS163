#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_
#include <QString>
#include <vector>

#include "ternary_seach_tree.h"

constexpr size_t h1_val = 1009;

class Hash_Table_Bucket
{
private:
	Word word;
    QString keyword;
	size_t hash = 0, len = 0;
	size_t other_hash = 0;

public:
	Hash_Table_Bucket() = default;

    Hash_Table_Bucket(QString keyword_, size_t hash_, size_t other_hash_, Word word_);

	bool operator==(const Hash_Table_Bucket& other);

	bool operator<(const Hash_Table_Bucket& other);

	bool operator>(const Hash_Table_Bucket& other);

	friend class Hash_Table;
};

class Hash_Table
{
private:
	std::vector<Hash_Table_Bucket> table[h1_val];

    bool sorted[h1_val] = { 0 };

    size_t hashing_1(QString s);

    long long hashing_2(QString s);

	size_t bucket_binary_search(std::vector<Hash_Table_Bucket>& v, 
		Hash_Table_Bucket& b, size_t begin, size_t end);

    void insertion_sort(std::vector<Hash_Table_Bucket>& v, size_t left, size_t right);
    void heapify(std::vector<Hash_Table_Bucket>& v, size_t index, size_t right);
    void heap_sort(std::vector<Hash_Table_Bucket>& v, size_t left, size_t right);
    size_t partition(std::vector<Hash_Table_Bucket>& v, size_t left, size_t right);
    void introsort(std::vector<Hash_Table_Bucket>& v, size_t left, size_t right, size_t depth);

    // Sort the hash table
    void sort(std::vector<Hash_Table_Bucket>& v);

	// Add a word to the hash_table
    void add_to_table_helper(QString keyword, Word word, bool load = false);

	// Remove a word from the hash_table
    void remove_from_table_helper(QString keyword, Word word);

    std::vector<Word> find_by_keyword(QString keyword);
public:
    Hash_Table() = default;

	Hash_Table(const Hash_Table& other);

    Hash_Table& operator=(const Hash_Table& other);

	~Hash_Table() = default;

	friend class Dictionary;
};

#endif
