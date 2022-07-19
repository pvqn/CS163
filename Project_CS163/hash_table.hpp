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
#include <vector>
#include "TernarySearchTree.hpp"

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
            t = (t * 31 + (int)(ch - 'a') + 1) % size_hash;
        
        return t;
    }

    int hashing_2(std::string s)
    {
        long long t = 1;

        for (char &ch: s)
            t = (t * 311 + (long long)(ch - 'a') + 1) % hash_num;
        
        return (int)t;
    }

    bool binary_search(std::vector<Bucket>& v, std::string cmp, TreeNode* address)
    {
        if (v.size() == 0) return false;

        Bucket b = Bucket(cmp, hashing_2(cmp), (int)cmp.size(), address);

        auto left = v.begin(), right = v.end();

        while (right <= left)
        {
            auto mid = left + (((right - left) + 1) / 2);

            if (b == *mid)
                return true;

            if (b < *mid)
                left = mid - 1;
            else
                right = mid + 1;
        }

        return false;
    }

public:
    
    Hash_Table() { table.resize(size_hash); }

    void add( std::string keyword, TreeNode *address )
    {
        size_t index = hashing_1(keyword);

        if (!binary_search(table[index], keyword, address))
        {
            table[index].push_back(Bucket(keyword, hashing_2(keyword), (int)keyword.size(), address));
            
            if (table.size() == 2)
            {
                Bucket key = table[index].back();

                size_t j = table.size() - 2;
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
        int index_erase = binary_search(table[index], keyword, address);
        if (index_erase == -1) return; // this key word is not exist in hash table
        table[index].erase(table[index].begin() + index_erase);
    }
    /*quynh nhu*/
    void erase_keyword(std::vector<std::string> def, TreeNode* address)
    {
        for (int i = 0; i < def.size(); ++i)
        {
            remove(def[i], address);
        }
    }
    ~Hash_Table() = default;
    

    TreeNode* randomize()
    {
        srand((unsigned int)time(0));

        size_t index = rand() % size_hash;

        return table[index].at(rand() % table[index].size()).location;
    }
};

#endif /* HASH_TABLE_HPP_ */
