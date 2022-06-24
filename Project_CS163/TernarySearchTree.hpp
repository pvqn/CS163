//
//  TernarySearchTree.hpp
//  TST
//

#ifndef TERNARY_SEARCH_TREE_HPP_
#define TERNARY_SEARCH_TREE_HPP_

#include <queue>
#include <string>
#include <utility>
#include <algorithm>

struct TreeNode
{
    char data = {};
    int high = 1; // length from this node to leaf node ( except for middle way )
    std::string def = {};

    TreeNode *left = nullptr;
    TreeNode *mid = nullptr;
    TreeNode *right = nullptr;

    TreeNode() = default;

    TreeNode(const char &_data, std::string _def) : data(_data), def(_def){};
};

class TernarySearchTree
{
private:
    TreeNode *root = nullptr;

    TreeNode *insert(TreeNode *root, char *s, std::string &def)
    {
        if (*s == '\0')
            return nullptr;

        if (!root)
        {
            root = new TreeNode(*s, ((*(s + 1) == '\0') ? def : ""));
            root->mid = insert(root->mid, s + 1, def);
            return root;
        }

        if (root->data == *s)
            root->mid = insert(root->mid, s + 1, def);

        if (root->data > *s)
            root->left = insert(root->left, s, def);

        if (root->data < *s)
            root->right = insert(root->right, s, def);
        root->high = std::max(getHigh(root->left), getHigh(root->right)) + 1;
        balance(root);
        return root;
    }

    TreeNode* searchNode(TreeNode* pRoot, std::string key, size_t index)
    {
        if (!pRoot) return pRoot;
        TreeNode* result = nullptr;
        if (key[index + 1] == '\0') if (!pRoot->def.empty()) return pRoot; else return result;
        if (key[index] < pRoot->data) return searchNode(pRoot->left, key, index);
        if (key[index] == pRoot->data) return searchNode(pRoot->mid, key, index + 1);
        if (key[index] > pRoot->data) return searchNode(pRoot->right, key, index);
    }

    int deleteNode(TreeNode *&root, size_t index, std::string s)
    {
        if (!root)
            return 0;
        if (s[index + 1] == '\0')
        {
            // if the string is in the tst
            if (!root->def.empty())
            {
                return (!root->left && !root->right && !root->mid);
            }
            else
                return 0;
        }
        else
        {
            if (s[index] < root->data)
                return deleteNode(root->left, index, s);
            else if (s[index] < root->data)
                return deleteNode(root->right, index, s);
            else if (s[index] == root->data)
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

    int getHigh(TreeNode* pRoot)
    {
        if (!pRoot) return 0; else return pRoot->high;
    }

    void rotation(TreeNode*& pRoot, int direct) // 0 left || 1 right
    {
        if ( direct == 0 ) // rotate to the left
        {
            TreeNode* new_root = pRoot->right;
            pRoot->right = new_root->left;
            pRoot->high = std::max(getHigh(pRoot->left), getHigh(pRoot->right)) + 1;
            new_root->left = pRoot;
            pRoot = new_root;
            pRoot->high = std::max(getHigh(pRoot->left), getHigh(pRoot->right)) + 1;
            return;
        }
        if (direct == 1) // rotate to the right
        {
            TreeNode* new_root = pRoot->left;
            pRoot->left = new_root->right;
            pRoot->high = std::max(getHigh(pRoot->left), getHigh(pRoot->right)) + 1;
            new_root->right = pRoot;
            pRoot = new_root;
            pRoot->high = std::max(getHigh(pRoot->left), getHigh(pRoot->right)) + 1;
            return;
        }
    }

    void balance(TreeNode*& pRoot)
    {
        if( abs(getHigh(pRoot->left) - getHigh(pRoot->right)) <= 1 ) return;
        if (getHigh(pRoot->left) > getHigh(pRoot->right))
        {
            if (getHigh(pRoot->left->right) > getHigh(pRoot->left->left)) rotation(pRoot->left, 0);
            rotation(pRoot, 1);
            return;
        }
        if (getHigh(pRoot->left) < getHigh(pRoot->right))
        {
            if (getHigh(pRoot->right->left) > getHigh(pRoot->right->right)) rotation(pRoot->right, 1);
            rotation(pRoot, 0);
            return;
        }
    }

    TreeNode *clone(TreeNode *current)
    {
        if (current == nullptr)
            return nullptr;

        TreeNode *new_node = new TreeNode;
        new_node->data = current->data;
        new_node->def = current->def;

        new_node->left = clone(current->left);
        new_node->mid = clone(current->mid);
        new_node->right = clone(current->right);

        return new_node;
    }

public:
    TernarySearchTree() = default;

    TernarySearchTree(const TernarySearchTree &other) noexcept
    {
        root = clone(other.root);
    }

    TernarySearchTree(TernarySearchTree &&other) noexcept : root(other.root)
    {
        other.root = nullptr;
    }

    TernarySearchTree &operator=(const TernarySearchTree &other) noexcept
    {
        if (this != &other)
        {
            TernarySearchTree copy = other;
            std::swap(copy.root, this->root);
        }

        return *this;
    }

    TernarySearchTree &operator=(TernarySearchTree &&other) noexcept
    {
        if (this != &other)
        {
            TernarySearchTree move = std::move(other);
            std::swap(move.root, this->root);
        }

        return *this;
    }

    void insert(std::string key, std::string def)
    {
        root = insert(root, &key[0], def);
    }

    void erase(std::string key)
    {
        deleteNode(root, 0, key);
    }

    TreeNode* search(std::string key)
    {
        return searchNode(root, key, 0);
    }
   
    /*
    void update(string key, string def) {

    }**/

    ~TernarySearchTree()
    {
        if (root)
        {
            std::queue<TreeNode *> q;
            q.push(root);

            while (!q.empty())
            {
                TreeNode *t = q.front();
                q.pop();
                if (t->left)
                    q.push(t->left);
                if (t->mid)
                    q.push(t->mid);
                if (t->right)
                    q.push(t->right);
                delete t;
            }

            root = nullptr;
        }
    }
};

#endif /* TERNARY_SEARCH_TREE_HPP_ */
