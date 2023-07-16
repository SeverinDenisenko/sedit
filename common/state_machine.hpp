//
// Created by Severin on 16.07.2023.
//

#ifndef SEDIT_STATE_MACHINE_HPP
#define SEDIT_STATE_MACHINE_HPP

#include <memory>
#include <functional>

template<typename T, typename U>
class state {
public:
    using state_ptr = std::shared_ptr<state>;
    using action_t = std::function<U(T)>;
    using state_switch_t = std::function<state_ptr(T)>;

    state(action_t action, state_switch_t state_switch) : action_(std::move(action)),
                                                          state_switch_(std::move(state_switch)) {
    }

    explicit state(state_switch_t state_switch) : state_switch_(std::move(state_switch)) {
    }

    U act(T t) {
        return action_(t);
    };

    state_ptr next(T t) {
        return state_switch_(t);
    };

private:
    action_t action_ = []([[maybe_unused]] char ch) {
        return U([]() {
        });
    };
    state_switch_t state_switch_;
};

template<typename T, typename U>
class state_machine {
public:
    using state_t = state<T, U>;
    using state_ptr = std::shared_ptr<state_t>;

    U apply(T t) {
        U res = current_->act(t);
        current_ = current_->next(t);
        return res;
    }

    void set_current(state_ptr current) {
        current_ = current;
    }

private:
    state_ptr current_;
};


#endif //SEDIT_STATE_MACHINE_HPP
