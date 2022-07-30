/**
 * hash_table.hpp
 *
 * Contains the hash table for keyword searching
 *
 */

#ifndef HASH_TABLE_HPP_
#define HASH_TABLE_HPP_
#include <cstddef>
#include <string>
#include "operation/util.hpp"
#include <vector>
#include "TernarySearchTree.hpp"
#include <QDebug>

const int size_hash = 1009;
const int hash_num = 1000000097;

class Hash_Table
{
private:

    struct Bucket
    {
        std::string word;                                       // keyword not word
        int key_hash_2, key_len;
        TreeNode* location = nullptr;
        Bucket(std::string keyword, int H2, int Length, TreeNode* loc)
            : word(keyword), key_hash_2(H2), key_len(Length), location(loc) {}

        bool operator==(const Bucket& other) const
        {
            return (key_hash_2 == other.key_hash_2)
                && (key_len == other.key_len)
                && (get_word(location) == get_word(other.location))
                && (word == other.word);
        }

        bool operator<(const Bucket& other) const
        {
            if (key_hash_2 != other.key_hash_2)
                return key_hash_2 < other.key_hash_2;
            if (key_len != other.key_len)
                return key_len < other.key_hash_2;
            if (word != other.word)
                return word < other.word;
            return (get_word(location) < get_word(other.location));
        }

        bool operator>(const Bucket& other) const
        {
            if (key_hash_2 != other.key_hash_2)
                return key_hash_2 > other.key_hash_2;
            if (key_len != other.key_len)
                return key_len > other.key_hash_2;
            if (word != other.word)
                return word > other.word;
            return (get_word(location) > get_word(other.location));
        }
    };


    std::vector<std::vector<Bucket>> table;

    size_t hashing_1(std::string s)
    {
        size_t t = 1;

        for (char &ch: s)
            t = (t * 311 + (int)(ch) + 1) % size_hash;

        return t;
    }

    long long hashing_2(std::string s)
    {
        long long t = 1;

        for (char &ch: s)
            t = (t * 769 + (long long)(ch) + 1) % hash_num;

        return t;
    }

    bool binary_search(std::vector<Bucket>& v, std::string cmp, TreeNode* address, bool status)
    {

        if (v.size() == 0) return false;

        Bucket b = Bucket(cmp, hashing_2(cmp), (int)cmp.size(), address);

        auto left = v.begin(), right = v.end();
        while (left <= right)
        {

            auto mid = left + (((right - left) + 1) / 2);

            if (b == *mid)
                return true;

            if (b < *mid)
                right = mid - 1;
            else
                left = mid + 1;
        }

        return false;
    }

    int lower_bound(std::vector<Bucket>& v, std::string cmp) {

        if (v.empty()) return -1;

        int l = 0, r = v.size()-1;

        int result = -1;

        int hashk2 = hashing_2(cmp);

        while (l <= r) {
            int mid = (l + r) / 2;

            if (v[mid].key_hash_2 == hashk2) {
                result = mid;
                r = mid-1;
            } else if (v[mid].key_hash_2 > hashk2) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }

        return result;
    }

    std::vector<std::string> findSomeInitialPrediction(std::string defx) {

        std::vector<std::string> result;

        int table_index = hashing_1(defx);

        int index = lower_bound(table[table_index], defx);

        if (index == -1) return result;

        int keyh2 = hashing_2(defx);

        while (index < table[table_index].size() || table[table_index][index].key_hash_2 == keyh2) {

            if (table[table_index][index].word == defx) {

                result.push_back(get_word(table[table_index][index].location));

            }

            ++index;

        }

        return result;
    }

    void filterOutPrediction(std::vector<std::string>& avPreds, std::string defx, TernarySearchTree& tst) {



        if (avPreds.empty()) return;

        int hashk1 = hashing_1(defx);

        std::vector<std::string> tmpAvPreds;

        for (std::string& str : avPreds) {

            if (binary_search(table[hashk1], defx, tst.searchNode(tst.root, str, 0), 1)) {

                tmpAvPreds.push_back(str);

            }

        }

        avPreds = tmpAvPreds;

    }

public:

    Hash_Table() { table.resize(size_hash); }

    void add( std::string keyword, TreeNode *address )
    {
        size_t index = hashing_1(keyword);

        if (!binary_search(table[index], keyword, address, 0))
        {

            table[index].push_back(Bucket(keyword, hashing_2(keyword), (int)keyword.size(), address));

            if (table[index].size() >= 2)
            {
                Bucket key = table[index].back();

                size_t j = table[index].size() - 2;

                bool neg = false;

                while (!neg && j >= 0 && table[index][j] > key)
                {

                    table[index][j + 1] = table[index][j];

                    if (j == 0) neg = true;
                    else --j;
                }

                table[index][neg ? 0 : j + 1] = key;
            }
        }
    }

    void remove( std::string keyword, TreeNode *address )
    {
        size_t index = hashing_1(keyword);
        int index_erase = binary_search(table[index], keyword, address, 1);
        if (index_erase == -1) return; // this key word is not exist in hash table
        table[index].erase(table[index].begin() + index_erase);
    }

     std::vector<std::string> getPrediction(std::string def, TernarySearchTree &tst) {

        std::vector<std::string> predic;

        for (auto& keyword : util::str::split(def)) {

            if (predic.empty()) {

                predic = findSomeInitialPrediction(keyword);

                if (predic.empty()) return predic;

            }

            else {

                filterOutPrediction(predic, keyword, tst);

                if (predic.empty()) return predic;

            }

        }

        if (predic.size() > 15) predic.resize(15);

        return predic;
    }

    /*quynh nhu*/
    void erase_keyword(std::vector<std::string> def, TreeNode* address)
    {
        for (int i = 0; i < def.size(); ++i)
        {
            remove(def[i], address);
        }
    }

    TreeNode* randomize()
    {
        srand((unsigned int)time(0));

        size_t index = rand() % size_hash;

        return table[index].at(rand() % table[index].size()).location;
    }

    ~Hash_Table() {
        for (int i = 0; i < size_hash; i++) {
            table[i].clear();
        }
        table.clear();
    }
};

#endif /* HASH_TABLE_HPP_ */
