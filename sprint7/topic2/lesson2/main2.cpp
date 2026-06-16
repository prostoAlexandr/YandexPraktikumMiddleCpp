#include <cassert>
#include <chrono>
#include <coroutine>
#include <exception>
#include <format>
#include <future>
#include <iostream>
#include <thread>
#include <print>

using namespace std::literals::chrono_literals;

void log(std::string_view msg) {
    std::stringstream id;
    id << std::this_thread::get_id();
    std::println("{} {}", id.str(), msg);
}

template <class T>
struct task {
    struct promise_type;

    std::future<T> future;
};

template <class T>
struct task<T>::promise_type {
    task get_return_object() { return {promise.get_future()}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_value(T value) {
        log(std::to_string(value));
        promise.set_value(value);
    }
    void unhandled_exception() { promise.set_exception(std::current_exception()); }

    std::promise<T> promise;
};

template <class T>
struct awaitable {

    awaitable(task<T> task) : future(std::move(task.future)) {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        std::thread([this, handle]() mutable {
            future.wait();
            log("+resume");
            handle.resume();
            log("-resume");
        }).detach();
    }

    T await_resume() { return future.get(); }

    std::future<T> future;
};

task<int> initialize_op() {
    log("initialize_op");
    co_return 42;
}

task<int> async_example() {
    log("+async_example");
    auto result = co_await awaitable{initialize_op()};
    for (int i = 0; i < 5; ++i) {
        log("Computing asynchronously...");
        std::this_thread::sleep_for(0.5s);
    }
    log("-async_example");
    co_return result + 1;
}

int main() {
    log("+main");
    auto task = async_example();
    task.future.wait();
    log("-main");
}