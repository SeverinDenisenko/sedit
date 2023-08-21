#include <iostream>

#include "application.hpp"
#include "state_machine.hpp"
#include "command_line_parser.hpp"

class viewer : public application {
public:
    explicit viewer(const std::string& filename) {
        term.params.cursor_visible = false;
        ansi::cursor::home();

        std::ifstream file(filename);
        if (!file) {
            std::cout << "Can't open file!" << std::endl;
            exit(1);
        }

        std::string str;
        logger.info("Open file " + filename + ": ");
        while (std::getline(file, str)) {
            text_.emplace_back(str);
            logger.info('\t' + str);
        }

        file.close();

        setupInputStateMachine();
    }

protected:
    command process(char ch) override {
        return input_state_machine_.apply(ch);
    }

    std::string render() override {
        std::string buffer;

        size_t begin = current_frame_ * term.window.columns;
        size_t end = begin + term.window.columns;

        for (size_t i = current_row_; i < term.window.rows + current_row_ - 1 && i < text_.size(); ++i) {
            size_t first = std::min(text_[i].size(), begin);
            size_t last = std::min(text_[i].size(), end);

            buffer += text_[i].substr(first, last - first);

            if (i != term.window.rows + current_row_ - 2)
                buffer += "\r\n";
        }

        return buffer;
    }

private:
    using input_machine_t = state_machine<char, command>;

    std::vector<std::string> text_;
    size_t current_row_ = 0;
    size_t current_frame_ = 0;
    input_machine_t input_state_machine_;

    input_machine_t::state_ptr nothing;
    input_machine_t::state_ptr control_begin;
    input_machine_t::state_ptr control_end;

    void setupInputStateMachine() {
        nothing = std::make_shared<input_machine_t::state_t>(
                [this](char ch) {
                    switch (ch) {
                        case esc & 'q':
                            logger.info("Received exit command.");
                            return command([this]() {
                                logger.info("Stopping.");
                                stop();
                            });
                        default:
                            return command([]() {
                                //
                            });
                    }
                },
                [this](char ch) {
                    if (ch == esc)
                        return control_begin;
                    else
                        return nothing;
                });

        control_begin = std::make_shared<input_machine_t::state_t>(
                [this](char ch) {
                    if (ch == '[')
                        return control_end;
                    else
                        return nothing;
                });

        control_end = std::make_shared<input_machine_t::state_t>(
                [this](char ch) {
                    switch (ch) {
                        case 'A':
                            return command([this]() {
                                if (current_row_ != 0)
                                    current_row_--;
                            });
                        case 'B':
                            return command([this]() {
                                if (current_row_ != text_.size())
                                    current_row_++;
                            });
                        case 'C':
                            return command([this]() {
                                current_frame_++;
                            });
                        case 'D':
                            return command([this]() {
                                if (current_frame_ != 0)
                                    current_frame_--;
                            });
                        default:
                            return command([]() {
                                //
                            });
                    }
                },
                [this]([[maybe_unused]] char ch) {
                    return nothing;
                });

        input_state_machine_.set_current(nothing);
    }
};


int main(int argc, char* argv[])
try {
    logger.enable = false;
    logger.init("sedit.log");

    command_line_parser parser(argc, argv);
    if (!parser.contains("-f")) {
        std::cerr << "No input provided!" << std::endl;
        return 1;
    }

    std::shared_ptr<application> app = std::make_shared<viewer>(parser.get("-f"));
    app->run();

    return 0;
} catch (std::exception& ex) {
    char_utils::print(ex.what());
}
