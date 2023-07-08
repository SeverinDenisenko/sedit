#include "terminal.hpp"
#include "char_utils.hpp"

void print(char c) {
    if (iscntrl(c)) {
        printf("%d\r\n", c);
    } else {
        printf("%d ('%c')\r\n", c, c);
    }
}

int main() {
    terminal t;
    ansi::cursor::hide();

    while (true) {
        char c = char_utils::get();
        print(c);
    }
}
