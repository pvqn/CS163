#include "ternary_seach_tree.h"

#include "util.h"

Word& Word::operator=(const Word& word)
{
	eow = word.eow;
	return *this;
}


std::string Word::get_definition() const
{
	return {};
}

// Get the word from the know Word struct
std::string Word::get_word() const
{
	return {};
}

bool Word::operator==(const Word& other)
{
	return get_word() == other.get_word();
}

bool Word::operator!=(const Word& other)
{
	return get_word() != other.get_word();
}

bool Word::operator<(const Word& other)
{
	return get_word() < other.get_word();
}

bool Word::operator>(const Word& other)
{
	return get_word() > other.get_word();
}

TST_Node* Ternary_Search_Tree::clone(TST_Node* other)
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

void Ternary_Search_Tree::destroy(TST_Node*& current)
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

Ternary_Search_Tree::~Ternary_Search_Tree()
{
	destroy(root);
}

Ternary_Search_Tree::Ternary_Search_Tree(const Ternary_Search_Tree & other)
{
	root = clone(other.root);
}

Ternary_Search_Tree& Ternary_Search_Tree::operator=(const Ternary_Search_Tree & other)
{
	if (this != &other)
	{
		Ternary_Search_Tree copy = other;
		util::algo::swap(copy.root, root);
	}
	return *this;
}

TST_Node* Ternary_Search_Tree::insert_helper(TST_Node* root, const std::string& word,
	const std::string& def, size_t index,
	TST_Node* parent,
	bool& valid)
{
	return nullptr;
}

TST_Node* Ternary_Search_Tree::search_helper(TST_Node* root, const std::string& word,
	size_t index)
{
	return nullptr;
}

bool Ternary_Search_Tree::remove_helper(TST_Node* root, const std::string& word,
	size_t index)
{
	return false;
}

void Ternary_Search_Tree::get_leaf_helper(TST_Node* root, std::vector<std::string>& result,
	size_t& count) {}

void Ternary_Search_Tree::print_helper(TST_Node* current, const char& separator,
	std::ostream& os) {}

void Ternary_Search_Tree::insert(std::string word, std::string def)
{
	bool is_valid = true;
	root = insert_helper(root, word, def, 0, nullptr, is_valid);

	if (is_valid)
		words_cache.push_back(Word(search_helper(root, word, 0)));
}

Word Ternary_Search_Tree::search(std::string word)
{
	return Word(search_helper(root, word, 0));
}

bool Ternary_Search_Tree::remove(std::string word)
{
	return remove_helper(root, word, 0);
}

void Ternary_Search_Tree::print(char separator, std::ostream& os)
{
	print_helper(root, separator, os);
}

std::vector<Word> Ternary_Search_Tree::get_words_list()
{
	return words_cache;
}

std::vector<std::string> Ternary_Search_Tree::get_prediction(std::string prefix)
{
	return {};
}
