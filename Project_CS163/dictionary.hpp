#ifndef dictionary_hpp
#define dictionary_hpp

#include <cstdio>
#include <iostream>
#include "TernarySearchTree.hpp"
#include "hash_table.hpp"
#include <fstream>
#include <vector>

class Dictionary {
private:
    int size = 0;
    
    TernarySearchTree data;
    Hash_Table table;
    std::vector< TreeNode* > favorite_list;              // contain pointer to the end of word on the tree
    std::vector< TreeNode* > history;
    std::string pathCurrentDataset;
    char def_delim;
    
public:

    friend class Dictionaries;

    Dictionary() : Dictionary("slang", '`') {}

    Dictionary(std::string file_name, char delim)
    {
        pathCurrentDataset = file_name + ".txt";
        def_delim = delim;
        load(file_name + ".txt", delim);
    }
    
    void load(std::string path, char delim_char) {
        pathCurrentDataset = ((path.substr(0,4) == "ORG_") ? path.substr(4, path.length()-3) : path);
        std::ifstream fin(path); //if (fin) std::cout << "OK" << '\n'; else std::cout << "NOPE" << '\n';
        std::string line;
        while (getline(fin, line)) {
            size_t delim = line.find(delim_char);

            std::string word = line.substr(0, delim);
            std::string def = line.substr(delim + 1, line.length() - delim - 1);

            data.insert(word, def);

            for (std::string& str : util::str::split(def))
            {
                table.add(str, nullptr);
            }

            ++size;
        }
        
        table.shrink_to_fit();

        fin.close();
    }
    
    void reset() {
        data.~TernarySearchTree();
        size = 0;
        load(pathCurrentDataset, def_delim);
    }

    void cache()
    {
        std::ofstream out("cache_" + pathCurrentDataset);

        if (out.is_open())
        {
            data.print_tree(def_delim, out);
        }
    }

    void remove(std::string word)
    {
        std::vector<std::string> keyword;
        TreeNode* eow = nullptr;

        data.erase(word, keyword, eow);

        for (std::string& str : keyword)
        {
            table.remove(str, eow);
        }
    }

    void insert(std::string key, std::string def)
    {
        data.insert(key, def);
    }

    void erase(std::string key, std::vector<std::string>& keywords, TreeNode* eow)
    {
        data.erase(key, keywords, eow);
    }

    void erase(std::string key)
    {
        data.erase(key);
    }

    TreeNode* search(std::string key)
    {
        return data.search(key, history);
    }

    void update(std::string key, std::string def)
    {
        data.update(key, def);
    }

    void print_dic()
    {
        std::ofstream iff("test.txt");
        data.print_tree( '`', iff);
        iff.close();
    }

    void view_favorite_list()
    {
        std::cout << "Favorite list: " << '\n';
        int cnt = 0;
        for( TreeNode* address : favorite_list ) std::cout << ++cnt << ' ' << get_word(address) << '\n';
    }
};

void actionOnFavList(std::string a, bool status);
void printOutHistory();
void randomWord4Def(Hash_Table def);
#endif /* dictionary_hpp */
