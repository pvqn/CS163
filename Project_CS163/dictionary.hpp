#ifndef dictionary_hpp
#define dictionary_hpp

#include <filesystem>
#include <cstdio>
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

    Dictionary(std::string file_name, char delim)
    {
        pathCurrentDataset = file_name + ".txt";
      
        if (!std::filesystem::exists(pathCurrentDataset) 
            && std::filesystem::exists("ORG_"+pathCurrentDataset))
            std::filesystem::copy_file("ORG_" + pathCurrentDataset, pathCurrentDataset);

        def_delim = delim;
        load(pathCurrentDataset, delim);
        print_dic();
    }
    
    void load(std::string path, char delim_char) {
        std::ifstream fin(path); //if (fin) std::cout << "OK" << '\n'; else std::cout << "NOPE" << '\n';
        
        if (fin)
        {
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
        }
    }
    
    void reset() {
        data.~TernarySearchTree();
        size = 0;

        std::filesystem::remove(pathCurrentDataset);
        std::filesystem::copy_file("ORG_" + pathCurrentDataset, pathCurrentDataset);

        load(pathCurrentDataset, def_delim);
    }
    
    void cache()
    {
        std::ofstream out(pathCurrentDataset);

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
        data.print_tree( def_delim, iff);
        iff.close();
    }

    void view_favorite_list()
    {
        std::cout << "Favorite list: " << '\n';
        int cnt = 0;
        for( TreeNode* address : favorite_list ) std::cout << ++cnt << ' ' << get_word(address) << '\n';
    }
    std::vector<TreeNode*> random4Word()
    {
       
        std::vector<TreeNode*> result;
        srand((unsigned int)time(0));
        std::vector<int> index;
        //random 4 number
        while (index.size() < 4)
        {
            int t = rand() % size ;
            bool check = false;
            for (int i=0; i<index.size();++i)
                if (t == index[i])
                {
                    check = true;
                    break;
                }
            if (!check) index.push_back(t);
        }
        //sort 4 number
        for (int i=0; i<index.size();++i)
            for (int j=i;j<index.size(); ++j)
                if (index[i] > index[j])
                {
                    int temp = index[i];
                    index[i] = index[j];
                    index[j] = temp;
                }
        std::vector<TreeNode*> temp = data.getListOfeow();
        for (int i = 0; i < 4; ++i)
            result.push_back(temp[index[i]]);
        /*for (int i = 0; i < result.size(); ++i)
        {
            std::cout << get_word(result[i]) << "\n";
        }*/
        return result;
    }
    std::vector<TreeNode*> get()
    {
       
        //return random4Word();
        
       
    }
};

void actionOnFavList(std::string a, bool status);
void printOutHistory();
void randomWord4Def(Hash_Table def);
#endif /* dictionary_hpp */
