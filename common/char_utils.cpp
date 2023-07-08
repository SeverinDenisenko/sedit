//
// Created by Severin on 05.07.2023.
//

#include "char_utils.hpp"
#include "ansi.hpp"

#include <unistd.h>

void char_utils::print(const std::string& string) {
    ssize_t size = string.size();

    if(write(STDOUT_FILENO, string.c_str(), size) != size)
        throw io_exception("can't print to screen");
}

char char_utils::get(std::atomic<bool>& force_release) {
    ssize_t code;
    char c;
    while ((code = read(STDIN_FILENO, &c, 1)) != 1) {
        if(force_release)
            return 0;
        if (code == -1 && errno != EAGAIN)
            throw io_exception("get char returned not EAGAIN errno");
    }
    return c;
}

char char_utils::get() {
    ssize_t code;
    char c;
    while ((code = read(STDIN_FILENO, &c, 1)) != 1) {
        if (code == -1 && errno != EAGAIN)
            throw io_exception("get char returned not EAGAIN errno");
    }
    return c;
}

bool char_utils::printable(char c) {
    auto ch = static_cast<unsigned char>(c);
    return std::isprint(ch);
}

bool char_utils::space(char c) {
    auto ch = static_cast<unsigned char>(c);
    return std::isspace(ch);
}
