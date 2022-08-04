#include "hash_table.h"

#include <algorithm>

#include "util.h"

constexpr long long h2_val = 1000000097;

Hash_Table_Bucket::Hash_Table_Bucket(QString keyword_, size_t hash_, size_t other_hash_, Word word_)
    : word(word_), keyword(keyword_), hash(hash_), other_hash(other_hash_)
{
	len = keyword_.size();
}

bool Hash_Table_Bucket::operator==(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return false;

	if (len != other.len)
		return false;

	if (keyword != other.keyword)
		return false;

	if (other_hash != other.hash)
		return false;

	return word == other.word;
}

bool Hash_Table_Bucket::operator<(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return hash < other.hash;

	if (len != other.len)
		return len < other.len;

	if (keyword != other.keyword)
		return keyword < other.keyword;

	if (other_hash != other.hash)
		return other_hash < other.other_hash;

	return word < other.word;
}

bool Hash_Table_Bucket::operator>(const Hash_Table_Bucket& other)
{
	if (hash != other.hash)
		return hash > other.hash;

	if (len != other.len)
		return len > other.len;

	if (keyword != other.keyword)
		return keyword > other.keyword;

	if (other_hash != other.hash)
		return other_hash > other.other_hash;

	return word > other.word;
}

size_t Hash_Table::hashing_1(QString s)
{
	size_t t = 1;

    for (QChar& ch : s)
    {
        t = (t * 311 + ch.unicode() + 1) % h1_val;
    }

	return t;
}

long long Hash_Table::hashing_2(QString s)
{
	long long t = 1;

    for (QChar& ch : s)
    {
        t = (t * 769 + ch.unicode() +1) % h2_val;
    }

	return t;
}

Hash_Table::Hash_Table(const Hash_Table& other) 
{
	for (size_t i = 0; i < h1_val; i++)
		table[i] = other.table[i];
}

Hash_Table& Hash_Table::operator=(const Hash_Table& other)
{
	if (this != &other)
	{
		for (size_t i = 0; i < h1_val; i++)
			table[i] = other.table[i];
	}

	return *this;
}

size_t Hash_Table::bucket_binary_search(std::vector<Hash_Table_Bucket>& v,
	Hash_Table_Bucket& b, size_t begin, size_t end)
{
	size_t left = begin, right = end;

	size_t mid = left + (right - left) / 2;

	while (left < right)
	{
		if (b == v[mid])
			return mid;
		else if (b < v[mid])
			right = mid;
		else
			left = mid + 1;

		mid = left + (right - left) / 2;
	}

	return left;
}

void Hash_Table::insertion_sort(std::vector<Hash_Table_Bucket> &v, size_t left,
                                size_t right)
{
  for (size_t i = left + 1; i <= right; i++)
  {
    Hash_Table_Bucket key = v[i];
    size_t j = i - 1;

    while (j > 0 && j >= left && v[j] > key)
    {
      v[j + 1] = std::move(v[j]);
      j--;
    }

    if (j == 0 && v[0] > key)
      v[1] = std::move(v[0]), j = 0;
    else
      j++;

    v[j] = std::move(key);
  }
}

void Hash_Table::heapify(std::vector<Hash_Table_Bucket> &v, size_t index,
                         size_t right) {
  size_t l = index * 2 + 1, r = index * 2 + 2;
  size_t temp = index;

  if (l <= right && v[l] < v[temp])
    temp = l;

  if (r <= right && v[r] < v[temp])
    temp = r;

  if (temp != index)
  {
    util::algo::swap(v[temp], v[index]);
    heapify(v, temp, right);
  }
}

void Hash_Table::heap_sort(std::vector<Hash_Table_Bucket> &v, size_t left,
                           size_t right)
{
  size_t size = right - left + 1;

  for (size_t i = left + (size - 1) / 2; i > 0 && i >= left; i--)
    heapify(v, i, right);

  if (left == 0)
    heapify(v, 0, right);

  for (size_t i = right; i > left; i--)
  {
    util::algo::swap(v[left], v[i]);
    heapify(v, left, i);
  }
}

size_t Hash_Table::partition(std::vector<Hash_Table_Bucket> &v, size_t left,
                             size_t right) {
  size_t mid = (right - left + 1) / 2 + left;

  size_t median;

  if ((v[left] < v[mid] && v[mid] < v[right]) ||
      (v[right] < v[mid] && v[mid] < v[left]))
    median = mid;
  else if ((v[left] < v[right] && v[right] < v[mid]) ||
           (v[mid] < v[right] && v[right] < v[left]))
    median = right;
  else
    median = left;

  util::algo::swap(v[median], v[right]);

  size_t pivot = left;

  while (left < right)
  {
    if (v[left] < v[right])
      util::algo::swap(v[pivot++], v[left]);

    ++left;
  }

  util::algo::swap(v[right], v[pivot]);
  return pivot;
}

void Hash_Table::introsort(std::vector<Hash_Table_Bucket> &v, size_t left,
                           size_t right, size_t depth) {
  if (left >= right)
    return;

  if (depth == 0)
  {
    heap_sort(v, left, right);
  } else if (right - left + 1 < 16)
  {
    insertion_sort(v, left, right);
  } else
  {
    size_t pivot = partition(v, left, right);

    introsort(v, left, pivot - 1, depth - 1);
    introsort(v, pivot + 1, right, depth - 1);
  }
}

void Hash_Table::sort(std::vector<Hash_Table_Bucket> &v)
{
  size_t depth = size_t(2 * log2(v.size()));
  introsort(v, 0, v.size() - 1, depth);
}

void Hash_Table::add_to_table_helper(QString keyword, Word word, bool load)
{
	size_t h1 = hashing_1(keyword);

    Hash_Table_Bucket b = Hash_Table_Bucket(keyword, hashing_2(keyword),
                                            hashing_2(word.get_word()), word);

	if (load)
	{
		table[h1].push_back(b);
	}
	else
	{
        if (sorted[h1] == 0)
        {
            std::sort(table[h1].begin(), table[h1].end());
            sorted[h1] = 1;
        }

		if (table[h1].empty())
			table[h1].push_back(b);
		else
		{
			size_t i = bucket_binary_search(table[h1], b, 0, table[h1].size() - 1);

			if (table[h1][i] == b) return;

			table[h1].push_back(b);

			if (table[h1][i] < b) i++;

			size_t j = table[h1].size() - 2;

			while (j > 0 && j >= i)
			{
				table[h1][j + 1] = std::move(table[h1][j]);
				j--;
			}

			if (j == 0 && table[h1][0] > table[h1][1])
			{
				table[h1][1] = std::move(table[h1][0]);
				j = 0;
			}
			else
			{
				j = j + 1;
			}

            table[h1][j] = std::move(b);
		}
	}
}

void Hash_Table::remove_from_table_helper(QString keyword, Word word)
{
	size_t h1 = hashing_1(keyword);

    Hash_Table_Bucket b = Hash_Table_Bucket(keyword, hashing_2(keyword),
                                            hashing_2(word.get_word()), word);

    if (sorted[h1] == 0)
    {
        std::sort(table[h1].begin(), table[h1].end());
        sorted[h1] = 1;
    }

    size_t i = bucket_binary_search(table[h1], b, 0, table[h1].size() - 1);

    if (table[h1][i] == b)
        table[h1].erase(table[h1].begin() + i);
}

std::vector<Word> Hash_Table::find_by_keyword(QString keyword)
{
	std::vector<Word> words;

    Hash_Table_Bucket b = Hash_Table_Bucket(keyword, hashing_2(keyword), 0, Word());

    size_t h1 = hashing_1(keyword);

    size_t index = bucket_binary_search(table[h1], b,
            0, table[h1].size() - 1);

    while (index < table[h1].size() && table[h1][index].keyword == keyword)
    {
        words.push_back(table[h1][index++].word);
    }

    return words;
}
