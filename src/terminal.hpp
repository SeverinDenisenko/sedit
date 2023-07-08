//
// Created by Severin on 05.07.2023.
//

#ifndef SEDIT_TERMINAL_HPP
#define SEDIT_TERMINAL_HPP

#include "ansi.hpp"

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace ansi;

class terminal_exception : public std::runtime_error {
public:
    explicit terminal_exception(const std::string& what) : std::runtime_error(what) {
    }
};

class terminal {
public:
    terminal();

    ~terminal();

protected:
    struct window_t {
        int rows;
        int columns;
    };

    window_t window{};
    ansi::cursor::position_t position = {1, 1};
    void update(const std::string& buffer);

    ansi::cursor::position_t getCursorPosition();
    window_t getWindow();

private:
    void setup();
    void shutdown();

    struct termios orig{};

    void loadRaw();
    void loadOriginal();
    void saveOriginal();
};


#endif //SEDIT_TERMINAL_HPP
