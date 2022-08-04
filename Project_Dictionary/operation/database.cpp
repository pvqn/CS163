#include "database.h"

#include <QDirIterator>

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
		}

        list.clear();
        list.push_back(current);
	}
}

void Database::change_dataset(QString file_name)
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

std::vector<QString> Database::get_databse_list()
{
    std::vector<QString> file_list;

    QString condition = "ORG_";

    QDirIterator it(QString(main_folder), QDirIterator::NoIteratorFlags);

    while (it.hasNext())
    {
        QString file_name = it.next();

        QFileInfo file(file_name);

        if (file.fileName().contains(condition))
        {
            file_list.push_back((file.fileName().mid(4, file.fileName().size() - 8)));
        }
    }

    return file_list;
}

Dictionary& Database::get()
{
	return *current;
}
