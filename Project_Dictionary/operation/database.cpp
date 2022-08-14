#include "database.h"

#include <vector>
#include <QDirIterator>

Database::~Database()
{
    delete current;
	current = nullptr;
}

void Database::change_dataset(QString file_name)
{
    delete current;
    current = new Dictionary(file_name);
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

void Database::reset()
{
    QString name = get().get_dataset_name();

    delete current;

    QFile::remove(main_folder + name + ".txt");  

    current = new Dictionary(name);
}
