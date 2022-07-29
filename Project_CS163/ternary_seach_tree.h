#ifndef TERNARY_SEARCH_TREE_H_
#define TERNARY_SEARCH_TREE_H_
#include <string>
#include <vector>
#include <ostream>
#include <cstddef>

class TST_Node
{
private:
	/**
	 * char: the letter of the word
	 * def: the full definition of the word
	 **/

	char data{};
	std::string def{};

	/**
	 * (Optional) The weight of each node
	 * for balance in a weight-balance form
	 **/

	unsigned int weight = 1;

	TST_Node* left = nullptr;
	TST_Node* right = nullptr;
	TST_Node* mid = nullptr;
	TST_Node* parent = nullptr;

public:
	TST_Node() = default;

	TST_Node(char data_, std::string def_) : data(data_), def(def_) {}

	~TST_Node() = default;

	// The full tree contains all the word
	friend class Ternary_Search_Tree;

	/**
	 * For getting and compare the word in the TST
	 **/
	friend class Word;
};

class Word
{
private:
	TST_Node* eow = nullptr;

public:
	Word() = default;

	Word(TST_Node* eow_) : eow(eow_) {}
	Word(const Word& word) : eow(word.eow) {}

	Word& operator=(const Word& word);

	// Get the definition from the know Word struct
	std::string get_definition() const;

	// Get the word from the know Word struct
	std::string get_word() const;

	bool operator==(const Word& other);

	bool operator!=(const Word& other);

	bool operator<(const Word& other);

	bool operator>(const Word& other);
};

class Ternary_Search_Tree
{
private:
	TST_Node* root = nullptr;
	std::vector<Word> words_cache;

	TST_Node* clone(TST_Node* other);

	void destroy(TST_Node*& current);

public:
	/**
	 * Constructor and destructor
	 * The struct is copyable but not movable
	 **/

	Ternary_Search_Tree() = default;

	~Ternary_Search_Tree();

	Ternary_Search_Tree(const Ternary_Search_Tree& other);

	Ternary_Search_Tree& operator=(const Ternary_Search_Tree& other);

private:
	/**
	 * TODO (optional): add rotation, set_weight and get_weight fuctions
	 * if there is time to balance the tree
	 **/

	 /**
	  * Backend function for adding a new word
	  *
	  * Checking the '\0' status by using the index of the word
	  **/
	TST_Node* insert_helper(TST_Node* root, const std::string& word,
		const std::string& def, size_t index, TST_Node* parent, bool& valid );

	// Searching a word backend
	TST_Node* search_helper(TST_Node* root, const std::string& word,
		size_t index);

	/**
	 * Remove a word in the tree
	 *
	 * Return bool for deleting process status
	 **/
	bool remove_helper(TST_Node* root, const std::string& word,
		size_t index);

	// Helper function for get the word prediction
	void get_leaf_helper(TST_Node* root, std::vector<std::string>& result,
		size_t& count);

	// Print the tree to a certain std::ostream
	void print_helper(TST_Node* current, const char& separator,
		std::ostream& os);

public:
	void insert(std::string word, std::string def);

	Word search(std::string word);

	bool remove(std::string word);

	void print(char separator, std::ostream& os);

	std::vector<Word> get_words_list();

	std::vector<std::string> get_prediction(std::string prefix);
};

#endif
