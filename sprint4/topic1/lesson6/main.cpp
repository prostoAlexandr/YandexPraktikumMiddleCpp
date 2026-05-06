#include <ranges>
#include <vector>
#include <print>

int main() {
    std::vector nums{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto result = std::views::transform(std::views::filter(nums, [](int val) { return val % 2 == 0; }),
                                        [](int val) { return val * val; });
    for (const auto &elem : result)
        std::print("{} ", elem);
}