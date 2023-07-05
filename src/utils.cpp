//
// Created by Severin on 05.07.2023.
//

#include "utils.hpp"

#include <unistd.h>

void utils::print(const std::string& string) {
    ssize_t size = string.size() + 1;

    if(write(STDOUT_FILENO, string.c_str(), size) != size)
        throw io_exception("can't print to screen");
}

char utils::get() {
    ssize_t code;
    char c;
    while ((code = read(STDIN_FILENO, &c, 1)) != 1) {
        if (code == -1 && errno != EAGAIN)
            throw io_exception("get char returned not EAGAIN errno");
    }
    return c;
}
