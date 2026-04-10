#include <type_traits>
#include <concepts>

template <typename T, typename... Ts>
concept SameAsFirst = (std::same_as<T, Ts> && ...);

template <typename T, typename... Ts>
requires SameAsFirst<T, Ts...>
auto add(T arg, Ts... args) { return (arg + ... + args); }

int main()
{
    return add(1, 2, 3);
}