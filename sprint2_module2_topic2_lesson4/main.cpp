#include <cassert>
#include <utility>

int fusedMultiplyAdd(int x, int y, int z) { return x * y + z; };

template <typename F, typename... Ts>
auto simple_bind_front(F &&func, Ts &&...boundArgs) {
    return
        [... boundArgs = std::forward<Ts>(boundArgs), func = std::forward<F>(func)](auto &&...args) -> decltype(auto) {
            // return func(std::forward<Ts>(boundArgs)..., std::forward<decltype(args)>(args)...);  // compilation error
            return func(boundArgs..., std::forward<decltype(args)>(args)...);
        };
}

void test_fma() {
    auto mul_42 = simple_bind_front(fusedMultiplyAdd, 42);
    auto result = mul_42(10, 5);
    assert(result == 425);
}

int main() { test_fma(); }