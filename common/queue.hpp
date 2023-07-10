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
    queue() = default;

    template<typename ...A>
    void emplace(A&&... args){
        std::lock_guard lk(m);
        queue_.emplace(std::forward<A>(args)...);
        cv.notify_all();
    }

    T pop() {
        std::lock_guard lk(m);
        T res = queue_.front();
        queue_.pop();
        cv.notify_all();
        return res;
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
    std::queue<T> queue_;
};

using command = std::function<void()>;
using command_queue = queue<command>;
using input_queue = queue<char>;

#endif //SEDIT_QUEUE_HPP
