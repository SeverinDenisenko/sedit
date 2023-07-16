//
// Created by Severin on 08.07.2023.
//

#ifndef SEDIT_QUEUE_HPP
#define SEDIT_QUEUE_HPP

#include <queue>
#include <mutex>

template<typename T>
class queue {
public:
    class queue_exception : public std::runtime_error {
    public:
        explicit queue_exception(const std::string& message) : std::runtime_error(message) {}
    };

    queue() = default;

    template<typename ...A>
    void emplace(A&& ... args) {
        std::unique_lock lk(m);
        queue_.emplace(std::forward<A>(args)...);
        cv.notify_all();
    }

    T pop() {
        std::unique_lock lk(m);

        if (queue_.empty())
            throw queue_exception("can't pop while nothing in the queue");

        T res = queue_.front();
        queue_.pop();

        return res;
    }

    void wait_for_data(bool& force_release) {
        if (force_release)
            return;

        std::unique_lock lk(m);

        cv.wait(lk, [this, &force_release]() { return !queue_.empty() || force_release; });
    }

private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<T> queue_;
};

using command = std::function<void()>;
using command_queue = queue<command>;
using input_queue = queue<char>;

#endif //SEDIT_QUEUE_HPP
