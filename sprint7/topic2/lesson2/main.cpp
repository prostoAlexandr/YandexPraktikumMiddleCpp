#include <cassert>
#include <coroutine>
#include <exception>

struct resumable {
    struct promise_type;

    using coro_handle = std::coroutine_handle<promise_type>;

    resumable(coro_handle handle) : handle_(handle) { assert(handle); }

    ~resumable() {
        if (handle_) {
            handle_.destroy();
        }
    }

    resumable(resumable &) = delete;
    resumable(resumable &&rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }

    bool resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

private:
    coro_handle handle_;
};

struct resumable::promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;

    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
};

int G = 0;

resumable foo() {
    G += 1;
    co_await std::suspend_always();
    G += 1;
}

int main() {
    assert(G == 0);
    // call
    auto t = foo();  // suspend
    assert(G == 0);
    t.resume();  // suspend
    assert(G == 1);
    t.resume();  // return
    assert(G == 2);
}