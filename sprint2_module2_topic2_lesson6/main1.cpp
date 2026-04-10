#include <print>
#include <utility>

double f(double) {
    std::println("f(double)");
    return {};
}

// Здесь первичный шаблон булевской переменной has_size_v
template <typename T, typename = void>
const bool has_size_v = false;

// А здесь частичная специализация шаблона переменной has_size_v
template <typename T>
const bool has_size_v<T, std::void_t<decltype(std::declval<T &>().size())>> = true;

template <typename Container, std::enable_if_t<has_size_v<Container>>* = nullptr>
auto f(Container const &cont) {
    std::println("f(Container const&)");
    return cont.size();
}

struct S {
    int size() const { return 43; }
};

int main() {
    auto x = f(42);
    auto y = f(S{});
}