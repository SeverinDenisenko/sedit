#include <iostream>

#include "ansi.hpp"

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdlib>

void fatal(const std::string& message) {
    std::cerr << message << std::endl;
    exit(1);
}

namespace utils {
    void print(const std::string& string) {
        write(STDOUT_FILENO, string.c_str(), string.size());
    }

    char get() {
        ssize_t code;
        char c;
        while ((code = read(STDIN_FILENO, &c, 1)) != 1) {
            if (code == -1 && errno != EAGAIN)
                fatal("read");
        }
        return c;
    }
}

namespace terminal {
    struct position {
        int row;
        int column;
    };

    struct window_t {
        int rows;
        int columns;
    };

    static struct termios orig;
    static int ttyfd = STDIN_FILENO;
    static window_t window;

    void setRaw() {
        termios raw = orig;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_cc[VMIN] = 5;
        raw.c_cc[VTIME] = 8;
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        raw.c_cc[VMIN] = 2;
        raw.c_cc[VTIME] = 0;
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 8;

        if (tcsetattr(ttyfd, TCSAFLUSH, &raw) < 0)
            fatal("can't set raw mode");
    }

    void setOriginal() {
        tcsetattr(ttyfd, TCSAFLUSH, &orig);
    }

    void getOriginal() {
        if (tcgetattr(ttyfd, &orig) < 0)
            fatal("can't get tty settings");
    }

    window_t getWindow() {
        winsize ws{};

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
            fatal("window size");

        return {ws.ws_row, ws.ws_col};
    }

    void shutdown() {
        setOriginal();
        std::cout << std::endl;
    }

    void setup() {
        if (!isatty(ttyfd))
            fatal("not on a tty");

        getOriginal();
        setRaw();

        window = getWindow();
    }

    void draw(){
        using namespace ansi::colors::rgb;
        using namespace ansi::cursor;

        for (int i = 0; i < window.rows; ++i) {
            for (int j = 0; j < window.columns; ++j) {
                utils::print(go(i, j));
                utils::print(bg(i * 256 / window.rows, j * 256 / window.columns, 124));
                utils::print(" ");
            }
        }

        utils::print(home());
    }
}

int main() {
    using namespace ansi;
    using namespace ansi::colors::small;
    using namespace ansi::colors;
    using namespace ansi::styles;

    terminal::setup();

    utils::print(alternate::on());
    utils::print(erase::screen());
    utils::print(cursor::home());
    utils::print(cursor::hide());

    while (true) {
        terminal::draw();
        char c = utils::get();

        switch (c) {
            case 'q':
                goto exit;
            default:
                break;
        }
    }

    exit:

    utils::print(alternate::off());

    terminal::shutdown();

    return 0;
}
