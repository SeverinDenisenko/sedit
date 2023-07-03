#include <iostream>

#include "ansi.hpp"

int main() {
    using namespace ansi;
    using namespace ansi::colors::small;
    using namespace ansi::colors;
    using namespace ansi::styles;

    std::cout << erase::screen();

    int k = 20;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << cursor::go(i, j);
            std::cout << rgb::bg(i * 256 / k, j * 256 / k, 124);
            std::cout << ' ';
        }
    }

    std::cout << std::endl;

    return 0;
}
