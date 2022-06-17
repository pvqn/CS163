//
//  TernarySearchTree.hpp
//  TST
//

#ifndef TernarySearchTree_hpp
#define TernarySearchTree_hpp

#include <queue>
#include <string>
#include <stdio.h>
using namespace std;

struct TreeNode {
    char data;
    string def;
    
    TreeNode* left;
    TreeNode* mid;
    TreeNode* right;
    
    TreeNode() : data(), def(""), left(nullptr), mid(nullptr), right(nullptr) {};
    
    TreeNode(char& _data, string _def) : data(_data), def(_def), left(nullptr), mid(nullptr), right(nullptr) {};
    
};

class TernarySearchTree {
private:
    TreeNode* root;
    
    TreeNode* insert(TreeNode* root, int index, string& s, string& def) {
        if (index == s.length()) return nullptr;
        
        if (!root) {
            root = new TreeNode(s[index], ((index == s.length()-1) ? def : ""));
            root->mid = insert(root->mid, index+1, s, def);
            return root;
        }
        
        if (root->data == s[index]) root->mid = insert(root->mid, index+1, s, def);
        
        if (root->data > s[index]) root->left = insert(root->left, index, s, def);
        
        if (root->data < s[index]) root->right = insert(root->right, index, s, def);
        
        return root;
    }
    
public:
    TernarySearchTree() : root(nullptr) {};
    
    void insert(string key, string def) {
        root = insert(root, 0, key, def);
    }
    
    /*void erase(string key) {
     
     }
     
     TreeNode* search(string key) {
         
     }
     
     void update(string key, string def) {
         
     }**/
    
    ~TernarySearchTree() {
        queue<TreeNode*> q;
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
