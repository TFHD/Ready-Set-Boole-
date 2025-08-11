#include <iostream>
#include <cstdlib>

int mutiplier(unsigned int a, unsigned int b) {
    int res = 0;
    while (b > 0) {
      if (1 & b) res += a;
      a <<= 1;
      b >>= 1;
    }
    return res;
}

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "need 2 parameters" << std::endl;
        return 1;
    }
    else
        std::cout << mutiplier(std::atoi(av[1]), std::atoi(av[2])) << std::endl;
    return 0;
}

