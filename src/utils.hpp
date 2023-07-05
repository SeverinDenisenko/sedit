//
// Created by Severin on 05.07.2023.
//

#ifndef SEDIT_UTILS_HPP
#define SEDIT_UTILS_HPP

#include <stdexcept>
#include <string>

namespace utils {
    class io_exception : public std::runtime_error {
    public:
        explicit io_exception(const std::string& what) : std::runtime_error(what) {
        }
    };

    void print(const std::string& string);

    char get();
}

#endif //SEDIT_UTILS_HPP
