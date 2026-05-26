#include <print>
#include <string>
#include <variant>

template <class... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};

void Print(std::variant<std::string, double, int> var) {

    // std::visit(
    //     [](auto &&v) {
    //         using T = std::decay_t<decltype(v)>;
    //         if constexpr (std::is_same_v<T, int>) {
    //             std::println("Получен int: {}", v);
    //         } else if constexpr (std::is_same_v<T, double>) {
    //             std::println("Получен double: {}", v);
    //         } else if constexpr (std::is_same_v<T, std::string>) {
    //             std::println("Получен std::string: {}", v);
    //         } else {
    //             static_assert(false, "Unsupported type");
    //         }
    //     },
    //     std::move(var));

    std::move(var).visit(Multilambda{[](auto &&v) { static_assert(false, "Unsupported type"); }, 
                          [](int v) { std::println("Получен int: {}", v); },
                          [](double v) { std::println("Получен double: {}", v); },
                          [](std::string &&v) { std::println("Получен std::string: {}", v); }});
}

int main() {}