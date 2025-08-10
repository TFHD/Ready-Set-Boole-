#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <iomanip>

double map(uint16_t x, uint16_t y) {
    uint32_t res = 0;
    for (int i = 0; i < 16; ++i) {
        res |= ((x >> i) & 1) << (2 * i + 1);
        res |= ((y >> i) & 1) << (2 * i);
    }
    return static_cast<double>(res) / (((long)1 << 32) - 1);
}

int main(int ac, char **av) {

    if (ac != 3) { std::cout << "need 2 parameters" << std::endl; return 1; }

    uint16_t x = std::atoi(av[1]);
    uint16_t y = std::atoi(av[2]);

    std::cout << std::setprecision(15) << map(x, y) << std::endl;
    return 0;
}