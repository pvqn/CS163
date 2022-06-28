/**
 * hash_table.hpp
 * 
 * Contains the hash table for keyword searching 
 * 
 */

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

    std::vector<std::vector<Bucket>> table(1009);

    int hashing_1(std::string s)
    {
        int t = 1;

        for (char &ch: s)
            t = (t * 31 + (ch - 'a') + 1) % 1009;
        
        return t;
    }

    int hashing_2(std::string s)
    {
        long long t = 1;

        for (char &ch: s)
            t = (t * 311 + (ch - 'a') + 1) % 1000000097;
        
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
        sort(table[index].begin(), table[index].end(), compare);
    }

    void remove( std::string keyword, TreeNode *address )
    {
        int index = hashing_1(keyword);
        int index_erase = binary_search(table[index], keyword, address);
        if (index_erase == -1) return; // this key word is not exist in hash table
        table[index].erase(table[index].begin() + index_erase);
    }

    ~Hash_Table() = default;
};

#endif /* HASH_TABLE_HPP_ */
