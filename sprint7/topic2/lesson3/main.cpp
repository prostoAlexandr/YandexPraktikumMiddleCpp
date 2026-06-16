#include <coroutine>
#include <iostream>
#include <string>

template <typename T>
class generator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    generator(handle_type h) : coro(h) {}
    ~generator() {
        if (coro)
            coro.destroy();
    }

    generator(const generator &) = delete;
    generator &operator=(const generator &) = delete;

    generator(generator &&other) noexcept : coro(other.coro) { other.coro = nullptr; }

    bool next() {
        if (!coro || coro.done())
            return false;
        coro.resume();
        return !coro.done();
    }

    T value() const { return coro.promise().current_value; }

private:
    handle_type coro;
};

template <class T>
struct generator<T>::promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;

    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    auto yield_value(T value) {
        current_value = value;
        return std::suspend_always{};
    }

    T current_value;
};

// Реализуйте корутину
generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        std::swap(a, b);
        b += a;
    }
}

int main() {
    auto fib = fibonacci();
    for (int i = 0; i < 10; ++i) {
        fib.next();
        std::cout << fib.value() << " ";
    }
    // Ожидаемый вывод: 0 1 1 2 3 5 8 13 21 34
    return 0;
}