//The Anh

#include "dictionary.hpp"

void actionOnFavList(std::string a, bool status) {
    std::string path = "user_favlist.txt"; //might change the path later
    std::fstream fst(path, std::ios::app); fst.close();
    std::fstream fst2("temp.txt", std::ios::app);
    fst.open(path, std::ios::in);
    std::string x; //temp string
    bool duplicated = false;
    while (getline(fst, x)) {
        if (a == x) {
            duplicated = true;
            if (!status) fst2 << x << "\n";
        }
        else fst2 << x << "\n";
    }
    fst.close();
    if (status == 0 && !duplicated) fst2 << a << "\n";
    fst2.close();
    const char *p = path.c_str();
    remove(p);
    rename("temp.txt", p);
}
