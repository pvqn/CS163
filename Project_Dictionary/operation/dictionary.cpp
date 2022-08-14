#include "dictionary.h"

#include <QTime>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QtGlobal>
#include <QRandomGenerator>

#include <vector>

#include "util.h"

Dictionary::Dictionary(QString dataset)
	: dataset_name(dataset)
{
    load();
}

Dictionary::Dictionary(const Dictionary& other)
	: dataset_name(other.dataset_name), delim(other.delim)
{
	keyword_table = other.keyword_table;
	word_tree = other.word_tree;
}

Dictionary::~Dictionary() { cache(); }

void Dictionary::load()
{
    if (!QFile::exists(main_folder + dataset_name + ".txt"))
        QFile::copy(main_folder + "ORG_" + dataset_name + ".txt",
            main_folder + dataset_name + ".txt");

    QFile file_fin(main_folder + dataset_name + ".txt");
    QFile stop_fin(main_folder + "stopwords.txt");

    if (file_fin.open(QIODevice::ReadOnly | QIODevice::Text)
            && stop_fin.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file_fin);
        QTextStream stop(&stop_fin);

        QString line = in.readLine();
        delim = line[0];

        Ternary_Search_Tree stopwords_tree;

        while (!stop.atEnd())
        {
            line = stop.readLine();

            for (QChar& ch: line)
            {
                ch = ch.toUpper();
            }

            Word eow; bool valid;

            stopwords_tree.insert_helper(stopwords_tree.root, line, " ",
                                         0, nullptr, valid, eow);
        }

        while (!in.atEnd())
        {
            line = in.readLine();

            qsizetype i = 0;

            while (i < line.size() && line[i] != delim)
                i++;

            if (i >= line.size()) continue;

            QString word = line.left(i);
            QString def = line.sliced(i + 1);

            bool is_valid = false;

            Word w;

            word_tree.root = word_tree.insert_helper(word_tree.root, word, def, 0, nullptr, is_valid, w);

            if (is_valid)
            {
                word_tree.words_cache.push_back(w);

                std::vector<QString> hashing(def.size());

                for (const QString& str : util::str::split(def))
                {
                    size_t h = (7 + 31 * str.front().unicode()
                                + 31 * 31 + str.back().unicode()) % hashing.size();

                    for (size_t i = 0; i < hashing.size(); i++)
                    {
                      if (!hashing[(h + i) % hashing.size()].isEmpty())
                      {
                        if (hashing[(h + i) % hashing.size()] == str) break;
                      }
                      else
                      {
                        hashing[(h + i) % hashing.size()] = str;
                        keyword_table.add_to_table_helper(str, w, true);
                        break;
                      }
                    }
                }
            }
        }
    }
}

void Dictionary::cache()
{
    QFile file(main_folder + dataset_name + ".txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        out << delim << '\n';

        word_tree.print_helper(word_tree.root, delim, out);
    }
}

void Dictionary::action_on_favorite_file(QString word, bool status)
{
    QString path = main_folder + "FAV_" + dataset_name + ".txt";
    QString temp_file = main_folder + "temp.txt";

    QFile original_f(path);
    QFile temp_f(temp_file);

    if (original_f.open(QIODevice::ReadWrite | QIODevice::Text)
            && temp_f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream original(&original_f), temp(&temp_f);

        QString str;
        bool duplicate = false;

        while (!original.atEnd())
        {
            str = original.readLine();

            if (word == str)
            {
                duplicate = true;
                if (!status) temp << str << '\n';
            }
            else
                temp << str << '\n';
        }

        if (!status && !duplicate) temp << word << '\n';
    }

    original_f.close();
    temp_f.close();

    if (original_f.remove())
        temp_f.rename(path);
}

void Dictionary::pushing_into_history_file(QString word)
{
    QDate set_time = QDate::currentDate();

    QFile file(main_folder + "HIS_" + dataset_name + ".txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QTextStream his(&file);

        int year, month, day;

        set_time.getDate(&year, &month, &day);

        his << year << '~' << month << '~' << day << '~' << word << '\n';
    }
}

bool Dictionary::dataset_is_equal(QString name)
{
    return name == dataset_name;
}

QString Dictionary::get_dataset_name()
{
    return dataset_name;
}

size_t Dictionary::get_dictionary_size()
{
    return word_tree.words_cache.size();
}

void Dictionary::insert(QString word, QString def)
{
    bool is_valid = false;

    Word w;

    word_tree.root = word_tree.insert_helper(word_tree.root, word, def, 0, nullptr, is_valid, w);

    if (is_valid)
    {
        QString find;

        word_tree.words_cache.push_back(w);

        QFile sw_f(main_folder + "stopwords.txt");

        if (sw_f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream sw(&sw_f);

            QString temp;

            while (true)
            {
                temp = sw.readLine();

                if (temp.isNull()) break;

                find += find.isEmpty() ? "" : " " + temp.toUpper();
            }
        }

        for (const QString& keyword : util::str::split(def))
        {
            if (!find.contains(keyword))
                keyword_table.add_to_table_helper(keyword, w);
        }
    }
}

void Dictionary::remove(QString word)
{
    TST_Node* search = word_tree.search_helper(word_tree.root, word, 0);

    if (search)
    {
        Word w = Word(search);

        QString def = w.get_definition();

        for (const QString& str : util::str::split(def))
            keyword_table.remove_from_table_helper(str, w);

        for (size_t i = 0; i < word_tree.words_cache.size() - 1; i++)
        {
            if (w == word_tree.words_cache[i])
            {
                util::algo::swap(word_tree.words_cache[i], word_tree.words_cache.back());
                break;
            }
        }

        word_tree.words_cache.pop_back();

        word_tree.remove_helper(word_tree.root, word, 0);
    }
}

void Dictionary::edit_definition(QString word, QString def)
{
    TST_Node *node = word_tree.search_helper(word_tree.root, word, 0);

    Word w = Word(node);

    if (w.get_word().isEmpty() || w.get_definition() == def) return;

    for (const QString& keyword_old : util::str::split(w.get_definition()))
        keyword_table.remove_from_table_helper(keyword_old, w);

    for (const QString& keyword_new : util::str::split(def))
        keyword_table.add_to_table_helper(keyword_new, w);

    word_tree.update_def_helper(node, def);
}

Word Dictionary::search_for_definition(QString word, bool add_to_history)
{
    if (add_to_history) pushing_into_history_file(word);

    return Word(word_tree.search_helper(word_tree.root, word, 0));
}

std::vector<Word> Dictionary::search_for_definition(QString keyword)
{
    return keyword_table.find_by_keyword(keyword);
}

std::vector<Word> Dictionary::get_favorite_list()
{
    std::vector<Word> fav_list;
    QFile file(main_folder + "FAV_" + dataset_name + ".txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        QString t;

        while (!in.atEnd())
        {
            t = in.readLine();

            Word temp = Word(word_tree.search_helper(word_tree.root, t, 0));

            if (!temp.get_word().isEmpty()) fav_list.push_back(temp);
        }
    }

    return fav_list;
}

std::vector<QString> Dictionary::get_history_list()
{
    std::vector<QString> history;

    QFile file(main_folder + "HIS_" + dataset_name + ".txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        QString t;

        while (!in.atEnd())
        {
            t = in.readLine();

            history.push_back(t);
        }
    }

    return history;
}

void Dictionary::clear_history()
{
    if (QFile::exists(main_folder + "HIS_" + dataset_name + ".txt"))
        QFile::remove(main_folder + "HIS_" + dataset_name + ".txt");
}

std::vector<Word> Dictionary::random_words(size_t n)
{
    std::vector<Word> words = word_tree.words_cache;

    if (words.size() < n) return {};

    std::vector<bool> existed(words.size(), 0);

    std::vector<Word> randomWords;

    size_t count = 0;

    while (count < n)
    {
        size_t i = QRandomGenerator::global()->generate() % words.size();

        if (existed[i] == 0)
        {
            existed[i] = 1;
            randomWords.push_back(words[i]);
            count++;
        }
    }

    return randomWords;
}

std::vector<QString> Dictionary::get_word_prediction(QString prefix)
{
    if (prefix.isEmpty()) return {};

    return word_tree.get_prediction_helper(prefix);
}

std::vector<QString> Dictionary::get_keyword_prediction(QString prefix)
{
    std::vector<QString> result;

    QString hashing_check[10000];
    size_t freq_check[10000] = {0};

    std::vector<QString> spl = util::str::split(prefix);

    for (const QString& str : spl)
    {
        if (result.size() == 15) break;
        for (const Word& w : keyword_table.find_by_keyword(str))
        {
            if (result.size() == 15) break;
            QString s = w.get_word();

            size_t hashing = (1 + 31 * s.front().unicode()
                              + 31 * 31 * s.back().unicode()) % 10000;

            for (size_t i = 0; i < 10000; i++)
            {
                if (!hashing_check[(i + hashing) % 10000].isEmpty())
                {
                    if (hashing_check[(i + hashing) % 10000] == s)
                    {
                        freq_check[(i + hashing) % 10000]++;

                        if (freq_check[i] == spl.size())
                        {
                            if (result.size() < 15)
                                result.push_back(hashing_check[i]);
                        }

                        break;
                    }
                }
                else
                {
                    hashing_check[(i + hashing) % 10000] = s;
                    freq_check[(i + hashing) % 10000] = 1;
                    break;
                }
            }
        }
    }

    for (size_t i = 0; i < 10000; i++)
    {
        if (freq_check[i] == spl.size())
            result.push_back(hashing_check[i]);
    }

    if (result.size() > 15)
        result.resize(15);

    return result;
}
