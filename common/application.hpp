//
// Created by Severin on 16.07.2023.
//

#ifndef SEDIT_APPLICATION_HPP
#define SEDIT_APPLICATION_HPP

#include <future>

#include "terminal.hpp"
#include "queue.hpp"
#include "logger.hpp"

class application{
public:
    void run();

    virtual ~application() = default;

protected:
    virtual command process([[maybe_unused]] char ch);

    virtual std::string render();

    void stop();

    terminal term;

private:
    command_queue commands_;
    input_queue inputs_;
    bool exit_ = false;

    static void reader(application& app);

    static void input_processor(application& app);

    static void command_processor(application& app);

    static void renderer(application& app);
};


#endif //SEDIT_APPLICATION_HPP
