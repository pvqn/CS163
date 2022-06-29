//
//  TernarySearchTree.hpp
//  TST
//

#ifndef TERNARY_SEARCH_TREE_HPP_
#define TERNARY_SEARCH_TREE_HPP_

#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include "util.hpp"

struct TreeNode
{
    char data = {};
    int high = 1; // length from this node to leaf node ( except for middle way )
    std::string def = {};

	TreeNode* left = nullptr;
	TreeNode* mid = nullptr;
	TreeNode* right = nullptr;
	TreeNode* parent = nullptr;

	TreeNode() = default;

	TreeNode(const char& _data, std::string _def) : data(_data), def(_def) {};

	friend std::string get_word(TreeNode* node)
	{
		if (!node) return {};

		std::string ans;
		ans += node->data;
		
		TreeNode* temp = node;
		
		while (temp)
		{
			if (temp->parent && temp->parent->mid == temp)
				ans += temp->data;
			temp = temp->parent;
		}

		return std::string(ans.rbegin(), ans.rend());
	}
};

class TernarySearchTree
{
private:
	TreeNode* root = nullptr;

	TreeNode* insert(TreeNode* root, char* s, std::string& def)
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
		{
			root->mid = insert(root->mid, s + 1, def);
			if (root->mid)
				root->mid->parent = root;
		}

		if (root->data > *s)
		{
			root->left = insert(root->left, s, def);
			if (root->left)
				root->left->parent = root;
		}

        	if (root->data < *s)
		{
			root->right = insert(root->right, s, def);
			if (root->right)
				root->right->parent = root;
		}


        root->high = std::max(getHigh(root->left), getHigh(root->right)) + 1;
        balance(root);
        return root;
    }

	TreeNode* searchNode(TreeNode* pRoot, std::string key, size_t index)
	{
		if (!pRoot)
			return pRoot;
		TreeNode* result = nullptr;
		if (key[index + 1] == '\0')
			if (!pRoot->def.empty())
				return pRoot;
			else
				return result;
		if (key[index] < pRoot->data)
			return searchNode(pRoot->left, key, index);
		if (key[index] == pRoot->data)
			return searchNode(pRoot->mid, key, index + 1);
		if (key[index] > pRoot->data)
			return searchNode(pRoot->right, key, index);
		return nullptr;
	}

	/*quynh nhu*/
    int deleteNode( TreeNode *&root, size_t index, std::string s, std::vector<std::string> &keyword, TreeNode *& eow )
    {
        if (!root) return 0;
        if (s[index + 1] == '\0') // at the end of the string
        {
            // if the string is in the tst
			if (!root->def.empty()) 
			{ 
				keyword.clear();
				keyword = std::move(util::str::split(root->def));
				eow = root;
				return (!root->left && !root->right && !root->mid); 
			}
            return 0;
        }
        if( s[index + 1] != '\0' ) // still in the string
        {
            if (s[index] < root->data) return deleteNode(root->left, index, s, keyword, eow);

            if (s[index] < root->data) return deleteNode(root->right, index, s, keyword, eow);

            if (s[index] == root->data)
            {
                if( deleteNode(root->mid, index + 1, s,keyword, eow) ) // this string is not the prefix of any others
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
		if (direct == 0) // rotate to the left
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
		if (abs(getHigh(pRoot->left) - getHigh(pRoot->right)) <= 1) return;
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

		TreeNode* new_node = new TreeNode;
		new_node->data = current->data;
		new_node->def = current->def;

		new_node->left = clone(current->left);
		new_node->mid = clone(current->mid);
		new_node->right = clone(current->right);

		return new_node;
	}

	TreeNode* destroy(TreeNode* current)
	{
		if (current)
		{
			current->left = destroy(current->left);
			current->mid = destroy(current->mid);
			current->right = destroy(current->right);

			delete current;
		}

		return nullptr;
	}
	/*quynh nhu*/
	void addHistorytoFile(std::string keyword)
	{
		std::ofstream fout;
		fout.open("history.txt", std::ios::app);
		fout << keyword << "\n";
		fout.close();
	}

public:
	TernarySearchTree() = default;

	TernarySearchTree(const TernarySearchTree& other) noexcept
	{
		root = clone(other.root);
	}

	TernarySearchTree(TernarySearchTree&& other) noexcept : root(other.root)
	{
		other.root = nullptr;
	}

	TernarySearchTree& operator=(const TernarySearchTree& other) noexcept
	{
		if (this != &other)
		{
			TernarySearchTree copy = other;
			std::swap(copy.root, this->root);
		}

		return *this;
	}

	TernarySearchTree& operator=(TernarySearchTree&& other) noexcept
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

	void erase(std::string key, std::vector<std::string> &keywords, TreeNode * eow)
	{
		 deleteNode(root, 0, key, keywords, eow);
	}

	void erase(std::string key)
	{
		std::vector<std::string> trash_vector;
		TreeNode* trash_node;
		deleteNode(root, 0, key, trash_vector, trash_node);
	}
	
	TreeNode* search(std::string key,std::vector<TreeNode*>&history)
	{
		TreeNode* temp = searchNode(root, key, 0);
		/*quynh nhu*/
		addHistorytoFile(key);
		history.push_back(temp); 
		// add search word to vector history in the case user want to
		// see all history and access to one of these words
		return temp;
	}

	void update(std::string key, std::string def)
	{
		TreeNode* search = searchNode(root, key, 0);

		if (search)
		{
			search->def = def;
		}
	}

	~TernarySearchTree()
	{
		root = destroy(root);
	}
};

void insertStopWord(TernarySearchTree& stopword);

#endif /* TERNARY_SEARCH_TREE_HPP_ */
