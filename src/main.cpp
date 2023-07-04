#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <queue>
#include <cstdlib>

#include "ansi.hpp"

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace ansi;

void fatal(const std::string& message) {
    std::cerr << message << std::endl;
    exit(1);
}

namespace terminal {
    struct window_t {
        int rows;
        int columns;
    };

    using namespace ansi::cursor;

    static struct termios orig;
    static int ttyfd = STDIN_FILENO;
    static window_t window;
    static position_t position;

    void setRaw() {
        termios raw = orig;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;

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

    position_t getCursorPosition() {
        utils::print(cursor::position());
        return cursor::get();
    }

    window_t getWindow() {
        winsize ws{};

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
            if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
                fatal("window size");

            position_t p = getCursorPosition();
            return {p.row, p.column};
        }

        return {ws.ws_row, ws.ws_col};
    }

    void shutdown() {
        setOriginal();
        utils::print(alternate::off());
    }

    void setup() {
        if (!isatty(ttyfd))
            fatal("not on a tty");

        getOriginal();
        setRaw();

        utils::print(alternate::on());
        utils::print(erase::screen());
        utils::print(cursor::home());
    }

    void draw() {
        using namespace ansi::colors::rgb;
        using namespace ansi::cursor;

        window = getWindow();
        std::string buffer;

        buffer += hide();

        // TODO

        buffer += go(position.row, position.column);
        buffer += show();

        utils::print(buffer);
    }
}

enum command{
    EXIT,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

template<typename T>
class queue {
public:
    queue() = default;

    void push(T t){
        std::lock_guard lk(m);
        queue_.push(t);
    }

    T pop(){
        std::lock_guard lk(m);
        T res = queue_.front();
        queue_.pop();
        return res;
    }

    bool empty(){
        return queue_.empty();
    }
private:
    std::mutex m;
    std::queue<T> queue_;
};

void editor()
try {
    using namespace colors::small;
    using namespace colors;
    using namespace styles;

    terminal::setup();
    queue<command> commands;

    std::future read = std::async(std::launch::async, [&commands](){
        while (true){
            char c = utils::get();

            switch (c) {
                case ctrl & 'q':
                    commands.push(EXIT);
                    return;
                case esc:
                    if (utils::get() != '[')
                        break;
                    switch (utils::get()) {
                        case 'A':
                            commands.push(UP);
                            break;
                        case 'B':
                            commands.push(DOWN);
                            break;
                        case 'C':
                            commands.push(RIGHT);
                            break;
                        case 'D':
                            commands.push(LEFT);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
    });

    std::future write = std::async(std::launch::async, [&commands](){
        using namespace std::chrono_literals;
        using namespace std::chrono;

        time_point<std::chrono::system_clock> t = system_clock::now();

        while (true){
            while (!commands.empty()){
                command c = commands.pop();

                switch (c) {
                    case EXIT:
                        terminal::shutdown();
                        return;
                    case UP:
                        if (terminal::position.row > 1)
                            terminal::position.row -= 1;
                        break;
                    case DOWN:
                        if (terminal::position.row < terminal::window.rows)
                            terminal::position.row += 1;
                        break;
                    case RIGHT:
                        if (terminal::position.column < terminal::window.columns)
                            terminal::position.column += 1;
                        break;
                    case LEFT:
                        if (terminal::position.column > 1)
                            terminal::position.column -= 1;
                        break;
                    default:
                        break;
                }
            }

            terminal::draw();
            t += 50ms;
            std::this_thread::sleep_until(t);
        }
    });

    read.wait();
    write.wait();
} catch (std::exception& ex){
    terminal::shutdown();

    utils::print(ex.what());
}

int main() {
    editor();

    return 0;
}
