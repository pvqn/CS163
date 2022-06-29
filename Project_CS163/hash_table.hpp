/**
 * hash_table.hpp
 * 
 * Contains the hash table for keyword searching 
 * 
 */

const int size_hash = 1009;
const int hash_num = 1000000097;
#ifndef HASH_TABLE_HPP_
#define HASH_TABLE_HPP_
#include <string>
#include <vector>
#include <algorithm>
#include "TernarySearchTree.hpp"

class Hash_Table
{
private:
   
    struct Bucket
    {
        std::string word;                                       // keyword not word
        int key_hash_2, key_len;
        TreeNode* location;                                     // address of the end of word
        Bucket( std::string keyword, int H2, int Length, TreeNode *loc) : word(keyword), key_hash_2(H2), key_len( Length ), location( loc ) {};
    };
    std::vector<std::vector<Bucket>> table = std::vector<std::vector<Bucket>> (size_hash);
    int hashing_1(std::string s)
    {
        int t = 1;

        for (char &ch: s)
            t = (t * 31 + (ch - 'a') + 1) % size_hash;
        
        return t;
    }

    int hashing_2(std::string s)
    {
        long long t = 1;

        for (char &ch: s)
            t = (t * 311 + (ch - 'a') + 1) % hash_num;
        
        return t;
    }

    bool compare(Bucket A, Bucket B)
    {
        if (A.key_hash_2 != B.key_hash_2) return A.key_hash_2 < B.key_hash_2;
        if (A.key_len != B.key_len) return A.key_len < B.key_len;
        return A.word < B.word;
    }

    int binary_search( std::vector<Bucket> v, std::string cmp, TreeNode *address )
    {
        Bucket consider = Bucket(cmp, hashing_2(cmp), cmp.size(), address);
        int L = lower_bound(v.begin(), v.end(), consider, compare) - v.begin();
        int R = upper_bound(v.begin(), v.end(), consider, compare) - v.begin();
        for (int i = L; i < R; ++i) if (v[i].location == address) return i;
        return -1;
    }

public:
    
    
    Hash_Table() = default;

    void add( std::string keyword, TreeNode *address )
    {
        int index = hashing_1(keyword);
        table[index].push_back(Bucket(keyword, hashing_2(keyword), keyword.size(), address));
        std::sort(table[index].begin(), table[index].end(), compare);
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
            int index1 = rand() % size_hash;;
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
};

#endif /* HASH_TABLE_HPP_ */
