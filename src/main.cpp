#include <iostream>
#include <future>

#include "terminal.hpp"
#include "queue.hpp"
#include "command_line_parser.hpp"
#include "logger.hpp"
#include "state_machine.hpp"

class application{
public:
    void run() {
        std::thread read(reader, std::ref(*this));
        std::thread convert(input_processor, std::ref(*this));
        std::thread process(command_processor, std::ref(*this));
        std::thread render(renderer, std::ref(*this));

        read.join();
        convert.join();
        process.join();
        render.join();
    }

    virtual ~application() = default;

protected:
    virtual command process([[maybe_unused]] char ch) {
        return []() {};
    }

    virtual std::string render() {
        return "";
    }

    void stop() {
        exit_ = true;
        inputs_.notify();
        commands_.notify();
    }

    terminal term;

private:
    command_queue commands_;
    input_queue inputs_;
    bool exit_ = false;

    static void reader(application& app){
        while (true) {
            char c = char_utils::get(app.exit_);
            if (app.exit_)
                break;

            app.inputs_.emplace(c);
        }

        logger.info("Reader exited.");
    }

    static void input_processor(application& app){
        while (true) {
            if (app.exit_)
                break;
            logger.info("Waiting for input.");
            app.inputs_.wait_for_data(app.exit_);
            logger.info("Done waiting for input.");
            if (app.exit_)
                break;

            char input = app.inputs_.pop();
            logger.info("Got data from inputs: " + std::to_string(input));
            auto res = app.process(input);
            logger.info("Got command for processor.");
            app.commands_.emplace(res);
            logger.info("Send command to processor.");
        }

        logger.info("Input processor exited.");
    }

    static void command_processor(application& app){
        while (true) {
            app.commands_.wait_for_data(app.exit_);
            if (app.exit_)
                break;

            auto command = app.commands_.pop();
            command();
        }

        logger.info("Command processor exited.");
    }

    static void renderer(application& app){
        using namespace std::chrono_literals;
        using namespace std::chrono;

        time_point<std::chrono::system_clock> t = system_clock::now();

        while (true) {
            if (app.exit_)
                break;

            std::string buffer = app.render();

            app.term.render(buffer);
            t += 50ms;
            std::this_thread::sleep_until(t);
        }

        logger.info("Renderer exited.");
    }
};

class viewer : public application {
public:
    explicit viewer(const std::string& filename) {
        term.cursor_visible = false;
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

        for (size_t i = current_row_; i < term.window.rows + current_row_ - 1 && i < text_.size(); ++i) {
            buffer += text_[i];
            buffer += "\r\n";
        }

        return buffer;
    }

private:
    using input_machine_t = state_machine<char, command>;

    std::vector<std::string> text_;
    size_t current_row_ = 0;
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
                            return command([]() {
                                // current_row_--;
                            });
                        case 'D':
                            return command([]() {
                                // current_row_++;
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
