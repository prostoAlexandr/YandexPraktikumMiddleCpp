#include <algorithm>
#include <limits>
#include <print>
#include <ranges>
#include <stdexcept>
#include <vector>

template <std::ranges::input_range R>
    requires std::numeric_limits<std::ranges::range_value_t<R>>::is_specialized
auto find_min_max_values(R &&r) {
    if (std::ranges::empty(r)) {
        throw std::invalid_argument("find_min_max_values called with an empty range.");
    }
    using T = std::ranges::range_value_t<R>;  // тип элемента
    using State = std::pair<T, T>;            // тип возвращаемого значения

    // инициализируем минимум максимально возможным значением, а максимум — минимально возможным
    State initial{std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest()};

    auto f = [](auto stat, const auto &elem) {
        stat.first = std::min(stat.first, elem);
        stat.second = std::max(stat.second, elem);
        return stat;
    };

    return std::ranges::fold_left(r, initial, f);
}

int main() {
    std::vector<int> vec = {1, 2, 3, -1, 10, -44};
    auto [min, max] = find_min_max_values(std::move(vec));
    std::println("Min: {}, max: {}", min, max);
}