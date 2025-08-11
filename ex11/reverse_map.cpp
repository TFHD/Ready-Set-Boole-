#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <tuple>

typedef std::tuple<uint16_t, uint16_t> tuple;

tuple reverse_map(uint32_t res) {

    uint16_t x = 0;
    uint16_t y = 0;
    for (int i = 0; i < 16; ++i) {
        x |= ((res >> (2 * i + 1)) & 1) << i;
        y |= ((res >> (2 * i)) & 1) << i;
    }
    return {x, y};
}


int main(int ac, char **av) {

    if (ac != 2) { std::cout << "need 1 parameter" << std::endl; return 1; }

    double val = std::strtod(av[1], 0);
    
    if (val > 1.0) val = 1.0;
    if (val < 0) val = 0.0;
    
    uint32_t z = static_cast<uint32_t>(val * 4294967295.0 + 0.5);
    tuple res = reverse_map(z);
    std::cout << std::get<0>(res) << " " << std::get<1>(res) << std::endl;
    return 0;
}