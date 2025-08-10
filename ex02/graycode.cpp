#include <iostream>
#include <cstdlib>

int gray_code(int n) {
    return n ^ (n >> 1);
}

int main(int ac, char **av) {
    if (ac != 2) {
        std::cout << "need 1 parameters" << std::endl;
        return 1;
    }
    else
        std::cout << gray_code(std::atoi(av[1])) << std::endl;
    return 0;
}