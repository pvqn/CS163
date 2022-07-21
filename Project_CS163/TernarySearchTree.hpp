//
//  TernarySearchTree.hpp
//  TST
//

#ifndef TERNARY_SEARCH_TREE_HPP_
#define TERNARY_SEARCH_TREE_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include "util.hpp"

struct TreeNode
{
	char data = {};
	int high = 1; // length from this node to leaf node ( except for middle way )
	int weight = 1; 
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
				ans += temp->parent->data;
			temp = temp->parent;
		}

		return std::string(ans.rbegin(), ans.rend());
	}
};

class TernarySearchTree
{
private:
	TreeNode* root = nullptr;
	std::vector<TreeNode*>words;

	TreeNode* insert(TreeNode*& root, char* s, std::string& def)
	{
		if (*s == '\0')
			return nullptr;

		if (!root)
		{
			root = new TreeNode(*s, (*(s + 1) == '\0' ? def : ""));

			if (*(s + 1) == '\0') words.push_back(root);

			root->mid = insert(root->mid, s + 1, def);

			if (root->mid) root->mid->parent = root;
		}
		else
		{
			if (root->data == *s)
			{
				if (*(s + 1) == '\0')
				{
					if (root->def.empty())
					{
						++root->weight;
						root->def = def;
						words.push_back(root);
					}
				}
				else
				{
					root->mid = insert(root->mid, s + 1, def);
				}
			}
			else if (root->data < *s)
			{
				root->right = insert(root->right, s, def);
				if (root->right)
				{
					root->right->parent = root;
					if (root->right->weight > root->weight) root = rotate_left(root);
				}
			}
			else
			{
				root->left = insert(root->left, s, def);
				if (root->left)
				{
					root->left->parent = root;
					if (root->left->weight > root->weight) root = rotate_right(root);
				}
			}
		}

		return root;
	}

	TreeNode* searchNode(TreeNode* pRoot, std::string key, size_t index)
	{
		if (!pRoot)
			return pRoot;
		//std::cout << pRoot->data << '\n';
		if (key[index + 1] == '\0')
		{
			if (pRoot->data == key[index])
			{
				if (!pRoot->def.empty()) return pRoot;
				else return nullptr;
			}
		}
		if (key[index] < pRoot->data)
			return searchNode(pRoot->left, key, index);
		if (key[index] == pRoot->data)
			return searchNode(pRoot->mid, key, index + 1);
		if (key[index] > pRoot->data)
			return searchNode(pRoot->right, key, index);
		return nullptr;
	}

	/*quynh nhu*/
	int deleteNode(TreeNode*& root, size_t index, std::string s, std::vector<std::string>& keyword, TreeNode*& eow)
	{
		if (!root) return 0;
		if (s[index + 1] == '\0') // at the end of the string
		{
			// if the string is in the tst
			if (!root->def.empty())
			{
				keyword.clear();
				keyword = util::str::split(root->def);
				eow = root;
				return (!root->left && !root->right && !root->mid);
			}
			return 0;
		}
		if (s[index + 1] != '\0') // still in the string
		{
			int result = 0;
			if (s[index] < root->data) result = deleteNode(root->left, index, s, keyword, eow);

			if (s[index] < root->data) result = deleteNode(root->right, index, s, keyword, eow);

			if (s[index] == root->data)
			{
				if (deleteNode(root->mid, index + 1, s, keyword, eow)) // this string is not the prefix of any others
				{
					delete root->mid;
					root->mid = nullptr;
					// delete root if root doesnt have children
					result = root->def.empty() && (!root->left && !root->mid && !root->right);
				}
			}
			//balance(root);
			return result;
		}
		return 0;
	}

	int getHigh(TreeNode* pRoot)
	{
		if (!pRoot) return 0; else return pRoot->high;
	}

	/*void rotation(TreeNode*& pRoot, int direct) // 0 left || 1 right
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
	}*/

	/*void balance(TreeNode*& pRoot)
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
	}*/


	int get_weight(TreeNode*& pRoot)
	{
		return ( pRoot != nullptr ) ? pRoot->weight : 0;
	}

	void set_weight(TreeNode*& pRoot)
	{
		if (!pRoot) return;
		pRoot->weight = get_weight(pRoot->mid); // + get_weight(pRoot->left) + get_weight(pRoot->right);
	}

	TreeNode* rotate_left(TreeNode* node)
	{
		TreeNode* ancient = node->parent;
		TreeNode* child = node->right;

		node->right = child->left;

		if (node->right) node->right->parent = node;

		child->left = node;
		node->parent = child;

		child->parent = ancient;

		if (ancient)
		{
			if (ancient->left == node) ancient->left = child;
			if (ancient->right == node) ancient->right = child;
			if (ancient->mid == node ) ancient->mid = child;
		}

		//set_weight(node); set_weight(child); set_weight(ancient);

		return child;
	}

	TreeNode* rotate_right(TreeNode* node)
	{
		TreeNode* ancient = node->parent;
		TreeNode* child = node->left;

		node->left = child->right;

		if (node->left) node->left->parent = node;

		child->right = node;
		node->parent = child;

		child->parent = ancient;

		if (ancient)
		{
			if (ancient->left == node) ancient->left = child;
			if (ancient->right == node) ancient->right = child;
			if (ancient->mid == node) ancient->mid = child;
		}

		//set_weight(node); set_weight(child); set_weight(ancient);

		return child;
	}



	TreeNode* clone(TreeNode* current)
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

	void recursive_output(TreeNode* node, std::ostream& out, const char delim, std::string str = {})
	{
		if (node)
		{
			recursive_output(node->left, out, delim, str);

			if (!node->def.empty())
			{
				out << str + node->data << delim << node->def << '\n';
			}

			recursive_output(node->mid, out, delim, str + node->data);
			recursive_output(node->right, out, delim, str);
		}
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
			util::algo::swap(copy.root, this->root);
		}

		return *this;
	}

	TernarySearchTree& operator=(TernarySearchTree&& other) noexcept
	{
		if (this != &other)
		{
			TernarySearchTree move = std::move(other);
			util::algo::swap(move.root, this->root);
		}

		return *this;
	}

	void insert(std::string key, std::string def)
	{
		root = insert(root, &key[0], def);
	}

	void erase(std::string key, std::vector<std::string>& keywords, TreeNode* eow)
	{
		deleteNode(root, 0, key, keywords, eow);
	}

	void erase(std::string key)
	{
		std::vector<std::string> trash_vector;
		TreeNode* trash_node;
		deleteNode(root, 0, key, trash_vector, trash_node);
	}

	TreeNode* search(std::string key, std::vector<TreeNode*>& history)
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

	void print_tree(const char delim, std::ostream& out)
	{
		recursive_output(root, out, delim);
	}

	std::vector<TreeNode*> getListOfeow()
	{
		return words;
	}

	~TernarySearchTree()
	{
		root = destroy(root);
	}
};

void insertStopWord(TernarySearchTree& stopword);

#endif /* TERNARY_SEARCH_TREE_HPP_ */
