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
    Hash_Table table;
    std::vector< TreeNode* > favorite_list;              // contain pointer to the end of word on the tree
    std::vector< TreeNode* > history;
    std::vector<TreeNode*> randomWords;
    int randomIndex;
    char def_delim;

public:


    TernarySearchTree data;
    std::string pathCurrentDataset;

    int size = 0;

    friend class Dictionaries;

    Dictionary(std::string file_name, char delim)
    {
        pathCurrentDataset = file_name + ".txt";

        if (!std::filesystem::exists("dataset/"+pathCurrentDataset)
            && std::filesystem::exists("dataset/ORG_"+pathCurrentDataset))
            std::filesystem::copy_file("dataset/ORG_" + pathCurrentDataset, "dataset/"+pathCurrentDataset);

        def_delim = delim;
        load(pathCurrentDataset, delim);
        //print_dic();
    }

    void load(std::string path, char delim_char) {
        std::ifstream fin("dataset/" + path); //if (fin) std::cout << "OK" << '\n'; else std::cout << "NOPE" << '\n';

        if (fin)
        {

            std::string line;
            while (getline(fin, line)) {
                size_t delim = line.find(delim_char);
                std::string word = line.substr(0, delim);
                std::string def = line.substr(delim + 1, line.length() - delim - 1);

                insert(word, def);

            }
        }

        fin.close();
    }

    void reset() {
        table.~Hash_Table();
        data.~TernarySearchTree();
        size = 0;

        data = TernarySearchTree();
        table = Hash_Table();

        std::filesystem::remove("dataset/" + pathCurrentDataset);
        std::filesystem::copy_file("dataset/ORG_" + pathCurrentDataset, pathCurrentDataset);

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

        --size;

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
        ++size;

        data.insert(key, def);


        for (std::string& str : util::str::split(def))
        {

            table.add(str, data.searchNode(data.root, key, 0));
        }


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

    void random4Word()
    {

        randomWords.clear();
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
            randomWords.push_back(temp[index[i]]);
        randomIndex=rand()% 4;

    }

   std::vector<TreeNode*> getVector()
   {

        return randomWords;
   }

   int getIndex ()
   {
        return randomIndex;
   }

    std::vector<std::string> predictionByWord(std::string prefix)
    {
        return data.predictionByWord(prefix);
    }

    std::vector<std::string> predictionByDef(std::string prefix)
    {
        return table.getPrediction(prefix, data);
    }
};

void actionOnFavList(std::string a, bool status)
;
void printOutHistory();
void randomWord4Def(Hash_Table def);
#endif /* dictionary_hpp */
