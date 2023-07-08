#include <iostream>
#include <chrono>
#include <future>

#include "terminal.hpp"
#include "command_queue.hpp"
#include "input_parser.hpp"

class editor {
public:
    explicit editor(const std::string& text){
        text_ = text;
    }

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

    command_queue<void> commands_;
    std::string text_;
    std::atomic<bool> exit_ = false;
    terminal term;

    void processControlSequence() {
        char c = char_utils::get(exit_);

        switch (c) {
            case '[': {
                c = char_utils::get(exit_);

                switch (c) {
                    case 'A':
                        commands_.emplace([this]() {
                            if (term.position.row > 1)
                                term.position.row -= 1;
                        });
                        break;
                    case 'B':
                        commands_.emplace([this]() {
                            if (term.position.row < term.window.rows)
                                term.position.row += 1;
                        });
                        break;
                    case 'C':
                        commands_.emplace([this]() {
                            if (term.position.column < term. window.columns)
                                term.position.column += 1;
                        });
                        break;
                    case 'D':
                        commands_.emplace([this]() {
                            if (term.position.column > 1)
                                term.position.column -= 1;
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
            char c = char_utils::get(exit_);
            if (exit_)
                return;

            if (char_utils::printable(c)) {
                commands_.emplace([c, this]() {
                    text_ += c;
                    term.position.column++;
                });
            } else {
                switch (c) {
                    case backspace:
                        commands_.emplace([this]() {
                            if (!text_.empty()) {
                                text_.pop_back();
                                term.position.column--;
                            }
                        });
                        break;
                    case newline:
                        commands_.emplace([this]() {
                            text_ += '\r';
                            text_ += '\n';
                            term.position.column = 1;
                            term.position.row++;
                        });
                        break;
                    case esc & 'q':
                        commands_.emplace([this]() {
                            exit_ = true;
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
            commands_.wait_for_data(exit_);
            commands_.pop();
            if (exit_)
                return;
        }
    });

    actor renderer = actor([this]() {
        using namespace std::chrono_literals;
        using namespace std::chrono;

        time_point<std::chrono::system_clock> t = system_clock::now();

        while (true) {
            if (exit_)
                return;

            std::string buffer;

            for(char i: text_){
                if (i == '\n')
                    buffer += '\r';
                buffer += i;
            }

            term.render(buffer);
            t += 50ms;
            std::this_thread::sleep_until(t);
        }
    });
};


int main(int argc, char* argv[])
try{
    input_parser parser(argc, argv);
    if(!parser.contains("-f")){
        std::cerr << "No input provided!" << std::endl;
        return 1;
    }

    std::ifstream file(parser.get("-f"));
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    editor ed(text);
    ed.run();

    return 0;
} catch (std::exception& ex) {
    char_utils::print(ex.what());
}
