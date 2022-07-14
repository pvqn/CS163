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
#include <algorithm>
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

        auto operator<=>(const Bucket& other) const
        {
            if (key_hash_2 != other.key_hash_2)
                return key_hash_2 <=> other.key_hash_2;
            if (key_len != other.key_len)
                return key_len <=> other.key_hash_2;
            if (word != other.word)
                return word <=> other.word;
            return (get_word(location) <=> get_word(other.location));
        }
    };
    std::vector<std::vector<Bucket>> table = std::vector<std::vector<Bucket>> (size_hash);
    int hashing_1(std::string s)
    {
        int t = 1;

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

    int binary_search( std::vector<Bucket> v, std::string cmp, TreeNode *address )
    {
        Bucket b = Bucket(cmp, hashing_2(cmp), (int)cmp.size(), address);

        size_t left = 0, right = v.size();

        while (right > left)
        {
            size_t mid = left + (right - left) / 2;

            if (b == v[mid]) return (int)mid;
            else if (b < v[mid])
                right = mid - 1;
            else
                left = mid + 1;
        }

        return (int) - 1;
    }

public:
    
    Hash_Table() = default;

    void add( std::string keyword, TreeNode *address )
    {
        int index = hashing_1(keyword);
        if (binary_search(table[index], keyword, address) != -1)
        {
            table[index].push_back(Bucket(keyword, hashing_2(keyword), keyword.size(), address));
            
            util::algo::full_vector_sort(table[index]);
        }
    }

    void remove( std::string keyword, TreeNode *address )
    {
        int index = hashing_1(keyword);
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
    std::vector<TreeNode*> random4Word()
    {
        std::vector<TreeNode*> result;
        srand(time(0));
        while (result.size() != 4)
        {
            int index1 = rand() % size_hash;
            if (!table[index1].empty())
            {
                int index2 = rand() % table[index1].size();
                bool check = false;
                for (int i = 0; i < result.size(); ++i)
                    if (table[index1][index2].location == result[i])
                    {
                        check = true;
                        break;
                    }
                if (!check) result.push_back(table[index1][index2].location);
            }
        }
        return result;
    }

    TreeNode* randomize()
    {
        srand(time(0));

        size_t index = rand() % size_hash;

        return table[index].at(rand() % table[index].size()).location;
    }
};

#endif /* HASH_TABLE_HPP_ */
