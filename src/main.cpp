#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <queue>

#include "terminal.hpp"

enum command {
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

    void push(T t) {
        std::lock_guard lk(m);
        queue_.push(t);
    }

    T pop() {
        std::lock_guard lk(m);
        T res = queue_.front();
        queue_.pop();
        return res;
    }

    bool empty() {
        return queue_.empty();
    }

private:
    std::mutex m;
    std::queue<T> queue_;
};

class editor : public terminal {
public:
    void run() {
        using namespace colors::small;
        using namespace colors;
        using namespace styles;

        std::future read = std::async(std::launch::async, [this]() {
            while (true) {
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

        std::future write = std::async(std::launch::async, [this]() {
            using namespace std::chrono_literals;
            using namespace std::chrono;

            time_point<std::chrono::system_clock> t = system_clock::now();

            while (true) {
                while (!commands.empty()) {
                    command c = commands.pop();

                    switch (c) {
                        case EXIT:
                            terminal::shutdown();
                            return;
                        case UP:
                            if (position.row > 1)
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

                update();
                t += 50ms;
                std::this_thread::sleep_until(t);
            }
        });

        read.wait();
        write.wait();
    }

private:
    queue<command> commands;
};


int main() {
    try {
        editor ed;
        ed.run();
    } catch (std::exception& ex) {
        utils::print(ex.what());
    }

    return 0;
}
