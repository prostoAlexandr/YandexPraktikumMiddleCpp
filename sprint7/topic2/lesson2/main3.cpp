#include <algorithm>
#include <chrono>
#include <coroutine>
#include <exception>
#include <fstream>
#include <future>
#include <iostream>
#include <print>
#include <thread>

using namespace std::literals::chrono_literals;

void log(std::string_view msg) {
    std::stringstream id;
    id << std::this_thread::get_id();
    std::println("{} {}", id.str(), msg);
}

// code here
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
    void return_void() { log("returning void"); promise.set_value(); }
    void unhandled_exception() { promise.set_exception(std::current_exception()); }

    std::promise<T> promise;
};

struct file_awaitable {

    file_awaitable(const std::string &filename)
        : m_filename(filename) {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        std::thread{[this, handle] {
            m_file = std::make_shared<std::fstream>(m_filename);
            handle.resume();
        }}.detach();
    }

    std::shared_ptr<std::fstream> await_resume() { return m_file; }

    std::string m_filename;
    std::shared_ptr<std::fstream> m_file;
};

task<void> read_file_async(const std::string &filename) {
    log("+read_file_async");
    auto file = co_await file_awaitable{filename};
    if (!file->is_open()) {
        co_return;
    }

    std::string line;
    while (std::getline(*file, line)) {
        log(line);
    }
    file->close();
    log("-read_file_async");
}

int main() {
    log("+main");
    auto task = read_file_async("example.txt");
    task.future.wait();
    log("-main");
    return 0;
}