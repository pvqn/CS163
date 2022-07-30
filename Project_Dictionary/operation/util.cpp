#include "util.hpp"
#include "TernarySearchTree.hpp"

std::string util::str::to_upper(std::string input)
{
    for (char& c : input)
        c = toupper(c);
    return input;
}

std::vector<std::string> util::str::split(std::string definition)
{
    std::vector<std::string> words;

    std::vector<char> stop = { ':', '\t', '\'', ' ',
                                ',', '.', '?', '!',
                                '@', '#', '(', ')',
                                '|' , ';', '"', '`', '\'',
                                '!', '$', '%', '^', '&', '*',
                                '-', '_', '+', '=', '{', '}',
                                '\\', '<', '>' };

    std::string temp;

    for (size_t i = 0; i < definition.size(); i++)
    {
        bool check = true;

        for (int j = 0; j < stop.size() && check; j++)
            check = (definition[i] != stop[j]);

        if (check)
        {
            temp.push_back(definition[i]);
        }
        else
        {
            if (!temp.empty())
                words.push_back(util::str::to_upper(temp));
            temp.clear();
        }

    }

    if (!temp.empty())
        words.push_back(util::str::to_upper(temp));

    return words;
}
