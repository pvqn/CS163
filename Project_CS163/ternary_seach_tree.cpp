#include "ternary_seach_tree.h"

#include "util.h"

Word& Word::operator=(const Word& word) // DONE
{
	eow = word.eow;
	return *this;
}

std::string Word::get_definition() const // NOT YET
{
	return eow->def;
}

// Get the word from the know Word struct
std::string Word::get_word() const // DONE
{

	if (!eow)
		return {};

	std::string ans = "";
	ans += eow->data;

	TST_Node* temp = eow;

	while (temp)
	{
		if (temp->parent && temp->parent->mid == temp)
			ans += temp->parent->data;
		temp = temp->parent;
	}
	return std::string(ans.rbegin(), ans.rend());
}

bool Word::operator==(const Word& other) // DONE
{
	return get_word() == other.get_word();
}

bool Word::operator!=(const Word& other) // DONE
{
	return get_word() != other.get_word();
}

bool Word::operator<(const Word& other) // DONE
{
	return get_word() < other.get_word();
}

bool Word::operator>(const Word& other) // DONE
{
	return get_word() > other.get_word();
}

TST_Node* Ternary_Search_Tree::clone(TST_Node* other) // DONE
{
	if (!other)
		return nullptr;

	TST_Node* new_node = new TST_Node;

	new_node->data = other->data;
	new_node->def = other->def;
	new_node->weight = other->weight;

	new_node->left = clone(other->left);
	new_node->right = clone(other->right);
	new_node->mid = clone(other->mid);

	return new_node;
}

void Ternary_Search_Tree::destroy(TST_Node*& current) // DONE
{
	if (current)
	{
		destroy(current->left);
		destroy(current->right);
		destroy(current->mid);

		delete current;
		current = nullptr;
	}
}

Ternary_Search_Tree::~Ternary_Search_Tree() // DONE
{
	destroy(root);
}

Ternary_Search_Tree::Ternary_Search_Tree(const Ternary_Search_Tree& other) // DONE
{
	root = clone(other.root);
}

Ternary_Search_Tree& Ternary_Search_Tree::operator=(const Ternary_Search_Tree& other) // DONE
{
	if (this != &other)
	{
		Ternary_Search_Tree copy = other;
		util::algo::swap(copy.root, root);
	}
	return *this;
}

TST_Node* Ternary_Search_Tree::insert_helper(TST_Node* root, const std::string& word, const std::string& def, size_t index, TST_Node* parent, bool& valid) // NOT YET
{
	if (word[index] == '\0')
		return nullptr;

	if (!root)
	{
		root = new TST_Node(word[index], (word[index + 1] == '\0') ? def : "");

		root->parent = parent;

		root->mid = insert_helper(root->mid, word, def, index + 1, root, valid);

		// set_weight(root);
		return root;
	}

	if (root->data == word[index])
	{
		root->mid = insert_helper(root->mid, word, def, index + 1, root, valid);
		// set_weight(root);
	}

	if (root->data > word[index])
	{
		root->left = insert_helper(root->left, word, def, index, root, valid);
		// if (root->left->weight > root->weight) root = rotate_right(root);
	}

	if (root->data < word[index])
	{
		root->right = insert_helper(root->right, word, def, index, root, valid);
		// if (root->right->weight > root->weight) root = rotate_left(root);
	}

	// set_weight(root);
	return root;
}

TST_Node* Ternary_Search_Tree::search_helper(TST_Node* root, const std::string& word, size_t index) // DONE
{
	if (!root)
		return root;
	// std::cout << pRoot->data << '\n';
	if (word[index + 1] == '\0')
	{
		if (root->data == word[index])
		{
			if (!root->def.empty())
				return root;
			else
				return nullptr;
		}
	}
	if (word[index] < root->data)
		return search_helper(root->left, word, index);
	if (word[index] == root->data)
		return search_helper(root->mid, word, index + 1);
	if (word[index] > root->data)
		return search_helper(root->right, word, index);
	return nullptr;
}

bool Ternary_Search_Tree::remove_helper(TST_Node* root, const std::string& word, size_t index, std::vector<std::string>& keywords, Word& cur) // DONE
{
	// TODO: Remove the word from the words_cache vector

	if (!root)
		return 0;
	if (word[index + 1] == '\0') // at the end of the string
	{
		// if the string is in the tst
		if (root->data != word[index])
		{
			while (root->data != word[index])
			{
				if (root->data > word[index])
					root = root->left;
				else
					root = root->right;
			}
		}
		if (!root->def.empty())
		{
			keywords = util::str::split(root->def);
			cur=Word(root);
			root->def = "";
			return !(root->left || root->right || root->mid);
		}
		return 0;
	}
	if (word[index + 1] != '\0') // still in the string
	{
		if (word[index] < root->data)
			remove_helper(root->left, word, index, keywords,cur);
		else if (word[index] > root->data)
			remove_helper(root->right, word, index,keywords,cur);
		else if (word[index] == root->data)
		{
			if (remove_helper(root->mid, word, index + 1,keywords,cur)) // this string is not the prefix of any others
			{
				delete root->mid;
				root->mid = nullptr;
				// delete root if root doesnt have children
				return root->def.empty() && !(root->left || root->right || root->mid);
			}
		}
	}
	return 1;
}

TST_Node* Ternary_Search_Tree::search_helper(TST_Node* root, std::string& prefix, size_t index) // DONE
{
	if (!root) return nullptr;
	if (index == prefix.size() - 1 && prefix[index] == root->data)
		return root;
	if (root->data == prefix[index])
		return search_helper(root->mid, prefix, index + 1);
	if (root->data < prefix[index])
		return search_helper(root->right, prefix, index);
	return search_helper(root->left, prefix, index);
}

void Ternary_Search_Tree::get_leaf_helper(TST_Node* root, std::vector<std::string>& result, size_t& count) // DONE
{
	if (root == nullptr || count == 0)
		return;
	if (!root->def.empty())
	{
		result.push_back(Word(root).get_word());
		--count;
	}
	get_leaf_helper(root->left, result, count);
	get_leaf_helper(root->mid, result, count);
	get_leaf_helper(root->right, result, count);
}

void Ternary_Search_Tree::print_helper(TST_Node* current, const char& separator, std::ostream& os) {} // NOT YET

void Ternary_Search_Tree::insert(std::string word, std::string def, bool& is_valid) // NOT YET
{
	root = insert_helper(root, word, def, 0, nullptr, is_valid);

	if (is_valid)
		words_cache.push_back(Word(search_helper(root, word, 0)));

	// TODO: sort the word_cache vector
}

Word Ternary_Search_Tree::search(std::string word) // DONE
{
	return Word(search_helper(root, word, 0));
}

bool Ternary_Search_Tree::remove(std::string word, std::vector<std::string>& keywords, Word& cur) // DONE
{
	return remove_helper(root, word, 0, keywords, cur);
}

void Ternary_Search_Tree::print(char separator, std::ostream& os) // DONE
{
	print_helper(root, separator, os);
}

std::vector<Word> Ternary_Search_Tree::get_words_list() // DONE
{
	return words_cache;
}

std::vector<std::string> Ternary_Search_Tree::get_prediction(std::string prefix) // DONE
{
	std::vector<std::string> result;
	size_t count_max = 15;
	TST_Node* current = search_helper(root, prefix, 0);
	if (!current->def.empty())
	{
		result.push_back(Word(current).get_word());
		--count_max;
	}
	get_leaf_helper(current->mid, result, count_max);
	return result;
}
