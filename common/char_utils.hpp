//
// Created by Severin on 05.07.2023.
//

#ifndef SEDIT_CHAR_UTILS_HPP
#define SEDIT_CHAR_UTILS_HPP

#include <stdexcept>
#include <string>

namespace char_utils {
    class io_exception : public std::runtime_error {
    public:
        explicit io_exception(const std::string& what) : std::runtime_error(what) {
        }
    };

    void print(const std::string& string);

    char get(std::atomic<bool>& force_release);
    char get();

    bool printable(char c);
    bool space(char c);
}

#endif //SEDIT_CHAR_UTILS_HPP
