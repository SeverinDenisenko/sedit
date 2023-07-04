//
// Created by Severin on 05.07.2023.
//

#ifndef SEDIT_UTILS_HPP
#define SEDIT_UTILS_HPP

#include <stdexcept>
#include <string>
#include <regex>
#include <unistd.h>

namespace utils {
    class cant_get_char_exception : public std::runtime_error {
    public:
        explicit cant_get_char_exception(const std::string& what) : std::runtime_error(what) {
        }
    };

    void print(const std::string& string) {
        write(STDOUT_FILENO, string.c_str(), string.size() + 1);
    }

    char get() {
        ssize_t code;
        char c;
        while ((code = read(STDIN_FILENO, &c, 1)) != 1) {
            if (code == -1 && errno != EAGAIN)
                throw cant_get_char_exception("get char returned not EAGAIN errno");
        }
        return c;
    }
}

#endif //SEDIT_UTILS_HPP
