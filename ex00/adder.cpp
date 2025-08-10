#include <iostream>
#include <cstdlib>

int adder(int a, int b) {
    while (b != 0) {
        int sum = a ^ b;
        int ret = (a & b) << 1;
        a = sum;
        b = ret;
    }
    return a;
}

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "need 3 parameters" << std::endl;
        return 1;
    }
    else
        std::cout << adder(std::atoi(av[1]), std::atoi(av[2])) << std::endl;
    return 0;
}
