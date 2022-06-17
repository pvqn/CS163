//
//  TernarySearchTree.hpp
//  TST
//

#ifndef TernarySearchTree_hpp
#define TernarySearchTree_hpp

#include <queue>
#include <string>
#include <stdio.h>


struct TreeNode {
    char data;
    std::string def;

    TreeNode* left;
    TreeNode* mid;
    TreeNode* right;

    TreeNode() : data(), def(""), left(nullptr), mid(nullptr), right(nullptr) {};

    TreeNode(char& _data, std::string _def) : data(_data), def(_def), left(nullptr), mid(nullptr), right(nullptr) {};

};

class TernarySearchTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* root, char* s, std::string& def) {
        if ( *s == '\0') return nullptr;
        
        if (!root) {
            root = new TreeNode(*s, ((*(s+1) == '\0') ? def : ""));
            root->mid = insert(root->mid, s+1, def);
            return root;
        }
        
        if (root->data == *s) root->mid = insert(root->mid, s+1, def);
        
        if (root->data > *s) root->left = insert(root->left, s, def);
        
        if (root->data < *s) root->right = insert(root->right, s, def);
        
        return root;
    }
    
    int deleteNode(TreeNode*& root, int index, std::string s)
    {
        if (!root)
            return 0;
        if (s[index + 1] == '\0')
        {
            //if the string is in the tst
            if (!root->def.empty())
            {
                return (!root->left && !root->right && !root->mid);
            }
            else return 0;
        }
        else
        {
            if (s[index] < root->data)
                return deleteNode(root->left, index, s);
            else
                if (s[index] < root->data)
                    return deleteNode(root->right, index, s);
                else
                    if (s[index] == root->data)
                    {
                        if (deleteNode(root->mid, index + 1, s))
                        {
                            delete root->mid;
                            root->mid = nullptr;
                            // delete root if root doesnt have children
                            return root->def.empty() && (!root->left && !root->mid && !root->right);
                        }
                    }
        }
        return 0;
    }

public:
    TernarySearchTree() : root(nullptr) {};

    void insert(std::string key, std::string def) {
        root = insert(root, &key[0], def);
    }

    void erase(std::string key) {
        deleteNode(root, 0, key);
    }

    /*TreeNode* search(string key) {

    }

    void update(string key, string def) {

    }**/

    ~TernarySearchTree() {
        std::queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode*& t = q.front(); q.pop();
            if (t->left) q.push(t->left);
            if (t->mid) q.push(t->mid);
            if (t->right) q.push(t->right);
            delete t;
        }
    }
};

#endif /* TernarySearchTree_hpp */
