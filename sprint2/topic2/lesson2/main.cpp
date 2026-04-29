

#include <print>
#include <utility>
template <typename Pred, typename... Args>
auto all_of(Pred &&pred, Args &&...args) {
    static_assert((std::is_invocable_r_v<bool, Pred, Args> && ...),
                  "Predicate cannot be invoked on one of the type in parameter pack");
    return (pred(std::forward<Args>(args)) && ...);
}

int main() {
    std::println("{}", all_of([](auto &&arg) { return std::is_integral_v<std::decay_t<decltype(arg)>>; }, 1, 2, true));
    std::println("{}", all_of([](auto &&arg) { return std::is_integral_v<std::decay_t<decltype(arg)>>; }, 3.14, 1, 2));
    std::println("{}", all_of([](auto &&arg) { return std::is_floating_point_v<std::decay_t<decltype(arg)>>; }, 1.0f,
                              2.5, 3.4f));

    std::println("{}", all_of([](auto &&arg) { return std::is_floating_point_v<std::decay_t<decltype(arg)>>; }, 1.0f,
                              false, 2.5f, 2.0));
}