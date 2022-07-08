#ifndef MAIN_DICTIONARY_H_
#define MAIN_DICTIONARY_H_
#include "dictionary.hpp"

class Dictionaries
{
private:
	std::vector<Dictionary*> arr;
	Dictionary* data = nullptr;
	bool rebuild_mode = true;

public:
	Dictionaries() = default;

	~Dictionaries()
	{
		if (rebuild_mode)
		{
			for (unsigned long long i = 0; i < arr.size(); i++)
				delete arr[i];
		}
		else
			delete data;
	}

	void switch_dataset(std::string new_dir)
	{
		if (rebuild_mode && new_dir != data->pathCurrentDataset)
		{
			delete data;
			data = new Dictionary();
		}
		else if (!rebuild_mode)
		{
			for (unsigned long long i = 0; i < arr.size(); i++)
			{
				if (arr[i]->pathCurrentDataset == new_dir)
					return;
			}

			arr.push_back(new Dictionary());
		}
	}
};

#endif // !MAIN_DICTIONARY
