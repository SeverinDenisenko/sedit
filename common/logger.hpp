//
// Created by Severin on 16.07.2023.
//

#ifndef SEDIT_LOGGER_HPP
#define SEDIT_LOGGER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

class logger {
public:
    void init(const std::string& file){
        if(!enable)
            return;

        inited = true;
        stream_ = std::ofstream(file);
    }

    template<typename T>
    void info(const T& msg) {
        log("[INFO]: ", msg);
    }

    template<typename T>
    void warning(const T& msg) {
        log("[WARNING]: ", msg);
    }

    template<typename T>
    void error(const T& msg) {
        log("[ERROR]: ", msg);
    }

    template<typename T>
    void log(const std::string& label, const T& msg) {
        using namespace std::chrono;

        if(!enable)
            return;

        if(!inited)
            return;

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

    bool enable = true;

private:
    std::mutex m;
    std::ofstream stream_;
    bool inited = false;
};

static inline logger logger;

#endif //SEDIT_LOGGER_HPP
