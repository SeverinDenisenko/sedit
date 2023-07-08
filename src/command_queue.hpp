//
// Created by Severin on 08.07.2023.
//

#ifndef SEDIT_COMMAND_QUEUE_HPP
#define SEDIT_COMMAND_QUEUE_HPP

#include <queue>
#include <mutex>


template<typename R>
class command {
public:
    using callback_type = std::function<R(void)>;

    explicit command(callback_type callback) : callback(std::move(callback)) {

    }

    void operator()() {
        callback();
    }

private:
    callback_type callback;
};

template<typename T>
class command_queue {
public:
    command_queue() = default;

    template<typename ...A>
    void emplace(A&&... args){
        std::lock_guard lk(m);
        queue_.emplace(std::forward<A>(args)...);
        cv.notify_all();
    }

    void pop() {
        std::lock_guard lk(m);
        command<T> res = queue_.front();
        queue_.pop();
        res();
        cv.notify_all();
    }

    bool empty() {
        std::lock_guard lk(m);
        return queue_.empty();
    }

    void wait_for_data(std::atomic<bool>& force_release) {
        std::unique_lock lk(m);

        cv.wait(lk, [this, &force_release]() { return !queue_.empty() || force_release; });
    }

private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<command<T>> queue_;
};


#endif //SEDIT_COMMAND_QUEUE_HPP
