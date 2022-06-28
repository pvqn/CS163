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

class Hash_Table
{
private:
    struct Bucket
    {
        std::string word;
        int key_hash_2;
        int key_len;
    };

    int hashing_1(std::string s)
    {
        int t = 1;

        for (char &ch: s)
            t = (t * 31 + (ch - 'a') + 1) % 1009;
        
        return t;
    }

    int hashing_2(std::string s)
    {
        int t = 1;

        for (char &ch: s)
            t = (t * 311 + (ch - 'a') + 1) % 1000000097;
        
        return t;
    }

    std::vector<std::vector<Bucket>> table(1009);

    /*bool binary_search(std::vector<Bucket> v, std::string cmp)
    {

    }*/

public:
    
    Hash_Table() = default;

    void add(std::string word)
    {

    }

    void remove(std::string word)
    {
        
    }

    ~Hash_Table() = default;
};

#endif /* HASH_TABLE_HPP_ */
