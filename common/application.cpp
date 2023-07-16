//
// Created by Severin on 16.07.2023.
//

#include "application.hpp"

void application::run() {
    std::thread read(reader, std::ref(*this));
    std::thread convert(input_processor, std::ref(*this));
    std::thread process(command_processor, std::ref(*this));
    std::thread render(renderer, std::ref(*this));

    read.join();
    convert.join();
    process.join();
    render.join();
}

command application::process([[maybe_unused]] char ch) {
    return []() {};
}

std::string application::render() {
    return "";
}

void application::stop() {
    exit_ = true;
    inputs_.notify();
    commands_.notify();
}

void application::reader(application& app) {
    while (true) {
        char c = char_utils::get(app.exit_);
        if (app.exit_)
            break;

        app.inputs_.emplace(c);
    }

    logger.info("Reader exited.");
}

void application::input_processor(application& app) {
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

void application::command_processor(application& app) {
    while (true) {
        app.commands_.wait_for_data(app.exit_);
        if (app.exit_)
            break;

        auto command = app.commands_.pop();
        command();
    }

    logger.info("Command processor exited.");
}

void application::renderer(application& app) {
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
