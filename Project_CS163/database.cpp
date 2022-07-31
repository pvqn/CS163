#include "database.h"

Database::~Database()
{
	for (Dictionary*& dictionary : list)
	{
		delete dictionary;
		dictionary = nullptr;
	}
	current = nullptr;
}

void Database::set_cache_mode(bool to_cache)
{
	cache_mode = to_cache;

	if (!cache_mode)
	{
		for (Dictionary*& dictionary : list)
		{
			if (dictionary != current) delete dictionary;
			dictionary = nullptr;
		}

		list.resize(1);
		list[0] = current;
	}
}

void Database::change_dataset(std::string file_name)
{
	if (cache_mode)
	{
		for (Dictionary* dictionary : list)
		{
			if (dictionary->dataset_is_equal(file_name))
			{
				current = dictionary;
				return;
			}
		}

		list.push_back(new Dictionary(file_name));
		current = list.back();
	}
	else
	{
		delete current;
		list.resize(1);
		current = list[0] = new Dictionary(file_name);
	}
}

Dictionary& Database::get()
{
	return *current;
}
