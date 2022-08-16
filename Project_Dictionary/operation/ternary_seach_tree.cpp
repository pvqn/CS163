#include "ternary_seach_tree.h"

#include "util.h"

Word& Word::operator=(const Word& word)
{
	eow = word.eow;
	return *this;
}

QString Word::get_definition() const
{
	return eow ? eow->def : "";
}

// Get the word from the know Word struct
QString Word::get_word() const
{
	if (!eow) return "";

    QString ans = "";
    ans.insert(0, eow->data);

	TST_Node* temp = eow;

	while (temp)
	{
		if (temp->parent && temp->parent->mid == temp)
            ans.insert(0, temp->parent->data);
		temp = temp->parent;
	}
    return ans;
}

bool Word::operator==(const Word& other)
{
    return get_word().compare(other.get_word(), Qt::CaseSensitive) == 0;
}

bool Word::operator!=(const Word& other)
{
    return get_word().compare(other.get_word(), Qt::CaseSensitive) != 0;
}

bool Word::operator<(const Word& other)
{
    return get_word().compare(other.get_word(), Qt::CaseSensitive) < 0;
}

bool Word::operator>(const Word& other)
{
    return get_word().compare(other.get_word(), Qt::CaseSensitive) > 0;
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

Ternary_Search_Tree::Ternary_Search_Tree(const Ternary_Search_Tree& other)
{
	root = clone(other.root);
}

Ternary_Search_Tree& Ternary_Search_Tree::operator=(const Ternary_Search_Tree& other)
{
	if (this != &other)
	{
		Ternary_Search_Tree copy = other;
		util::algo::swap(copy.root, root);
	}
	return *this;
}

TST_Node* Ternary_Search_Tree::rotate_left(TST_Node* root)
{
	TST_Node* parent = root->parent, * child = root->right;

	root->right = child->left;
	if (root->right) root->right->parent = root;

	child->left = root;
	root->parent = child;
	child->parent = parent;

	if (parent)
	{
        if (parent->left == root) parent->left = child;
        if (parent->mid == root) parent->mid = child;
        if (parent->right == root) parent->right = child;
	}

	return child;
}

TST_Node* Ternary_Search_Tree::rotate_right(TST_Node* root)
{
	TST_Node* parent = root->parent, * child = root->left;

	root->left = child->right;
	if (root->left) root->left->parent = root;

	child->right = root;
	root->parent = child;
	child->parent = parent;

	if (parent)
	{
        if (parent->left == root) parent->left = child;
        if (parent->mid == root) parent->mid = child;
        if (parent->right == root) parent->right = child;
	}

	return child;
}

unsigned int Ternary_Search_Tree::get_weight(TST_Node* root)
{
	return root ? root->weight : 0;
}

void Ternary_Search_Tree::set_weight(TST_Node* root)
{
	if (root)
        root->weight = get_weight(root->mid) + !root->def.isEmpty();
}

TST_Node* Ternary_Search_Tree::insert_helper(TST_Node* root, const QString& word,
    const QString& def, size_t index, TST_Node* parent, bool& valid, Word& eow)
{
	if (index == word.size()) return nullptr;

	if (!root)
	{
		valid = true;

		root = new TST_Node(word[index], index + 1 == word.size() ? def : "");
		root->parent = parent;
        if (!root->def.isEmpty()) eow = Word(root);
		root->mid = insert_helper(root->mid, word, def, index + 1, root, valid, eow);

		return root;
	}

	if (root->data == word[index])
	{
		if (index + 1 == word.size())
		{
            valid = root->def.isEmpty();
			if (valid)
			{
				root->def = def;
				eow = Word(root);
			}
		}
		else
		{
			root->mid = insert_helper(root->mid, word, def, index + 1, root, valid, eow);
		}

		set_weight(root);
	}
	else if (word[index] < root->data)
	{
		root->left = insert_helper(root->left, word, def, index, root, valid, eow);
		if (get_weight(root->left) > get_weight(root)) root = rotate_right(root);
	}
	else
	{
		root->right = insert_helper(root->right, word, def, index, root, valid, eow);
		if (get_weight(root->right) > get_weight(root)) root = rotate_left(root);
	}

	return root;
}

TST_Node* Ternary_Search_Tree::search_helper(TST_Node* root, const QString& word,
                                             size_t index)
{
    if (!root) return nullptr;
    if (index == word.size() - 1 && word[index] == root->data)
        return root;
    if (root->data == word[index])
        return search_helper(root->mid, word, index + 1);
    if (root->data < word[index])
        return search_helper(root->right, word, index);
    return search_helper(root->left, word, index);
}

void Ternary_Search_Tree::update_def_helper(TST_Node* root, QString new_def)
{
    root->def = new_def;
}

bool Ternary_Search_Tree::remove_helper(TST_Node*& root, const QString& word, size_t index)
{
	if (!root)
		return 0;
	if (index == word.size() - 1) // at the end of the string
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
        if (!root->def.isEmpty())
		{
			root->def.clear();
			set_weight(root);

			return !(root->left || root->right || root->mid);
		}
		return 0;
	}
	else // still in the string
	{
		if (word[index] < root->data)
			remove_helper(root->left, word, index);
		else if (word[index] > root->data)
			remove_helper(root->right, word, index);
		else if (word[index] == root->data)
		{
			if (remove_helper(root->mid, word, index + 1)) // this string is not the prefix of any others
			{
				delete root->mid;
				root->mid = nullptr;
				// delete root if root doesnt have children
                return root->def.isEmpty() && !(root->left || root->right || root->mid);
			}
			else
			{
				set_weight(root);

				bool left = get_weight(root->left) > get_weight(root);
				bool right = get_weight(root->right) > get_weight(root);

				if (left && right)
				{
					if (get_weight(root->left) >= get_weight(root->right))
					{
						root = rotate_right(root);
						root->right = rotate_left(root->right);
					}
					else
					{
						root = rotate_left(root);
						root->left = rotate_right(root->left);
					}
				}
				else if (left)
				{
					root = rotate_right(root);
				}
				else if (right)
				{
					root = rotate_left(root);
				}
			}
		}
	}
	return false;
}

void Ternary_Search_Tree::get_leaf_helper(TST_Node* root, std::vector<QString>& result, size_t& count)
{
	if (root == nullptr || count == 0)
		return;
    if (!root->def.isEmpty())
	{
		result.push_back(Word(root).get_word());
		--count;
	}
	get_leaf_helper(root->left, result, count);
	get_leaf_helper(root->mid, result, count);
	get_leaf_helper(root->right, result, count);
}

void Ternary_Search_Tree::print_helper(TST_Node* current, const QChar& separator, QTextStream& os) // DONR
{
	if (current)
	{
		print_helper(current->left, separator, os);

        if (!current->def.isEmpty())
            os << Word(current).get_word().toUtf8() << separator << current->def << '\n';

		print_helper(current->mid, separator, os);
		print_helper(current->right, separator, os);
	}
}

std::vector<QString> Ternary_Search_Tree::get_prediction_helper(QString prefix)
{
    std::vector<QString> result;
	size_t count_max = 15;
	TST_Node* current = search_helper(root, prefix, 0);
    if (current)
	{
        if (!current->def.isEmpty())
        {
            result.push_back(Word(current).get_word());
            --count_max;
        }
        get_leaf_helper(current->mid, result, count_max);
	}

	return result;
}
