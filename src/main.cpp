#include <iostream>
#include <chrono>
#include <future>

#include "terminal.hpp"
#include "command_queue.hpp"

class editor : public terminal {
public:
    void run() {
        using namespace colors::small;
        using namespace colors;
        using namespace styles;

        std::thread read(std::move(reader));
        std::thread process(std::move(processor));
        std::thread render(std::move(renderer));

        read.join();
        process.join();
        render.join();
    }

private:
    using actor = std::packaged_task<void()>;

    command_queue<void> commands;
    std::string text;
    std::atomic<bool> exit = false;

    void processControlSequence() {
        char c = char_utils::get(exit);

        switch (c) {
            case '[': {
                c = char_utils::get(exit);

                switch (c) {
                    case 'A':
                        commands.emplace([this]() {
                            if (position.row > 1)
                                terminal::position.row -= 1;
                        });
                        break;
                    case 'B':
                        commands.emplace([this]() {
                            if (position.row < window.rows)
                                position.row += 1;
                        });
                        break;
                    case 'C':
                        commands.emplace([this]() {
                            if (position.column < window.columns)
                                position.column += 1;
                        });
                        break;
                    case 'D':
                        commands.emplace([this]() {
                            if (terminal::position.column > 1)
                                terminal::position.column -= 1;
                        });
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    actor reader = actor([this]() {
        while (true) {
            char c = char_utils::get(exit);
            if (exit)
                return;

            if (char_utils::printable(c)) {
                commands.emplace([c, this]() {
                    text += c;
                    position.column++;
                });
            } else {
                switch (c) {
                    case backspace:
                        commands.emplace([this]() {
                            if (!text.empty()) {
                                text.pop_back();
                                position.column--;
                            }
                        });
                        break;
                    case newline:
                        commands.emplace([this]() {
                            text += '\r';
                            text += '\n';
                            position.column = 1;
                            position.row++;
                        });
                        break;
                    case esc & 'q':
                        commands.emplace([this]() {
                            exit = true;
                        });
                        break;
                    case esc:
                        processControlSequence();
                    default:
                        break;
                }
            }
        }
    });

    actor processor = actor([this]() {
        while (true) {
            commands.wait_for_data(exit);
            commands.pop();
            if (exit)
                return;
        }
    });

    actor renderer = actor([this]() {
        using namespace std::chrono_literals;
        using namespace std::chrono;

        time_point<std::chrono::system_clock> t = system_clock::now();

        while (true) {
            if (exit)
                return;

            update(text);
            t += 50ms;
            std::this_thread::sleep_until(t);
        }
    });
};


int main() {
    try {
        editor ed;
        ed.run();
    } catch (std::exception& ex) {
        char_utils::print(ex.what());
    }

    return 0;
}
