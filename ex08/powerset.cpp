#include <iostream>
#include <vector>
#include <cstdlib>

typedef std::vector<int> tab;
typedef std::vector<tab> d_tab;

bool isStringIsDigit(std::string &str) {

    for (int i = 0; i < (int)str.length(); i++)
        if (!std::isdigit(str[i]))
            return 0;
    return 42;
}

void printTab(d_tab res) {

    for (int i = 0; i < (int)res.size(); i++) {
        std::cout << "[";
        for (int j = 0; j < (int)res[i].size(); j++) {
            std::cout << res[i][j];
            if (j + 1 < (int)res[i].size())
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

d_tab powerset(tab &vec) {

    int nb_possibility = 1 << (int)vec.size();
    d_tab res;
    for (int i = 0; i < nb_possibility; i++) {
        tab subTab;
        for (int j = 0; j < (int)vec.size(); j++) {
            if ((i >> j) & 1)
                subTab.push_back(vec[j]);
        }
        res.push_back(subTab);
    }
    return res;
}

int main(int ac, char **av) {

    if (ac < 2) { std::cout << "need 1+ parameters" << std::endl; return 1; }

    tab vec;
    for (int i = 1; i < ac; i++) {
        std::string str(av[i]);
        if (!isStringIsDigit(str)) {
            std::cout << "The string isn't an int" << std::endl;
            return 1;
        }
        vec.push_back(std::atoi(av[i]));
    }

    d_tab res;
    res = powerset(vec);
    printTab(res);

    return 0;
}