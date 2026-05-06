#include <algorithm>
#include <iterator>
#include <print>
#include <ranges>
#include <set>
#include <vector>

// здесь может быть ваш код
void print_number(std::ranges::view auto& view) {
    // здесь ваш код
    if (view.empty())
    {
        std::println("No elements in range match the value");
    }
    else
    {
        std::println("Number of elements equal to '{}' is {}", *view.begin(), std::ranges::distance(view));
    }
}

// здесь может быть ваш код
void print_number_of_elements_equal_to(std::ranges::range auto& range, const auto& value) {
    // здесь ваш код
    auto [first, last] = std::ranges::equal_range(range.begin(), range.end(), value);
    auto subrange = std::ranges::subrange(first, last);
    print_number(subrange);
}

int main() {
    std::multiset<int> numbers = {1, 2, 3, 3, 3, 4, 5, 7, 8, 9};
    print_number_of_elements_equal_to(numbers, 3);
    print_number_of_elements_equal_to(numbers, 13);
    std::vector<double> vec{1.1, 2.2, 3.3, 3.3, 4.4, 5.5, 7.7, 8.8, 9.9};
    print_number_of_elements_equal_to(vec, 3.3);
    return 0;
}