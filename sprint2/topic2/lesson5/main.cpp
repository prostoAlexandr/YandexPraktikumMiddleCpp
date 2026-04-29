#include <functional>
#include <print>
#include <utility>

template <typename Func>
class RecursionHelper {
    Func func_;

public:
    explicit RecursionHelper(Func func) : func_(std::move(func)) {}
    template <typename... Args>
    decltype(auto) operator()(Args &&...args) {
        // auto recursive = [this](auto &&...args){return (*this)(std::forward<Args>(args)...);};
        // return func_(recursive, std::forward<Args>(args)...);
        return func_(*this, std::forward<Args>(args)...);
    }
};

template <typename Func>
auto make_recursive(Func &&func) {
    return RecursionHelper{std::forward<Func>(func)};
}

int main() {
    auto fib = make_recursive([](auto &self, int n) -> int { return (n <= 1) ? n : self(n - 1) + self(n - 2); });

    auto x = fib(7);
    std::println("Fib of 7 is {}", x);
}