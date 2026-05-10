#include <generator>
#include <print>
#include <ranges>
#include <vector>

template <std::ranges::input_range R1, std::ranges::input_range R2>
    requires std::same_as<std::ranges::range_value_t<R1>, std::ranges::range_value_t<R2>>
std::generator<std::ranges::range_value_t<R1>> merge_sorted(R1 &&r1, R2 &&r2) {
    auto it1 = std::ranges::begin(r1);
    auto end1 = std::ranges::end(r1);
    auto it2 = std::ranges::begin(r2);
    auto end2 = std::ranges::end(r2);

    while (it1 != end1 && it2 != end2) {
        if (*it1 < *it2) {
            co_yield *it1;
            ++it1;
        } else {
            co_yield *it2;
            ++it2;
        }
    }

    while (it1 != end1) {

        co_yield *it1;
        ++it1;
    }

    while (it2 != end2) {

        co_yield *it2;
        ++it2;
    }
}

int main() { std::println("{}", merge_sorted(std::vector<int>{1, 3, 5, 6, 7}, std::vector<int>{0, 2, 4, 8})); }