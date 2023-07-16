#include <iostream>
#include <chrono>
#include <future>
#include <unordered_set>
#include <numeric>

#include "terminal.hpp"
#include "queue.hpp"
#include "command_line_parser.hpp"

class logger {
public:
    explicit logger(const std::string& filename) {
        stream_ = std::ofstream(filename);
    }

    void info(const std::string& msg) {
        log("[INFO]: ", msg);
    }

    void warning(const std::string& msg) {
        log("[WARNING]: ", msg);
    }

    void error(const std::string& msg) {
        log("[ERROR]: ", msg);
    }

    void log(const std::string& label, const std::string& msg) {
        using namespace std::chrono;

        std::stringstream res;

        res << std::this_thread::get_id() << ' ';

        auto now = system_clock::now();
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        auto timer = system_clock::to_time_t(now);
        std::tm bt = *std::localtime(&timer);

        res << std::put_time(&bt, "%H:%M:%S");
        res << '.' << std::setfill('0') << std::setw(3) << ms.count() << ' ';
        res << label;
        res << msg;

        std::lock_guard lk(m);
        stream_ << res.str() << std::endl;
    }

private:
    std::mutex m;
    std::ofstream stream_;
};

static inline logger logger("sedit.log");

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

template<typename T>
class state {
public:
    using state_ptr = std::shared_ptr<state>;
    using action_t = std::function<command(T)>;
    using state_switch_t = std::function<state_ptr(T)>;

    state(action_t action, state_switch_t state_switch) : action_(std::move(action)),
                                                          state_switch_(std::move(state_switch)) {
    }

    explicit state(state_switch_t state_switch) : state_switch_(std::move(state_switch)) {
    }

    command act(T t) {
        return action_(t);
    };

    state_ptr next(T t) {
        return state_switch_(t);
    };

private:
    action_t action_ = []([[maybe_unused]] char ch) {
        return command([]() {
        });
    };
    state_switch_t state_switch_;
};

template<typename T>
class state_machine {
public:
    using state_t = state<T>;
    using state_ptr = std::shared_ptr<state_t>;

    command apply(T t) {
        command res = current_->act(t);
        current_ = current_->next(t);
        return res;
    }

    void set_current(state_ptr current) {
        current_ = current;
    }

private:
    state_ptr current_;
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
    using input_machine_t = state_machine<char>;

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
