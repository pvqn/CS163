#include "util.h"

std::vector<QString> util::str::split(QString definition)
{
    std::vector<QString> words;

    QString temp;

    for (QChar ch : definition)
	{
        if (!ch.isSpace() && !ch.isPunct())
            temp.push_back(ch.toUpper());
		else
		{
            if (!temp.isEmpty())
				words.push_back(temp);
			temp.clear();
		}
	}

    if (!temp.isEmpty())
		words.push_back(temp);

	return words;
}
