#include <ranges>
#include <vector>
#include <print>

struct Number {
    int i;
    Number(int val) : i(val){std::println("Constructor Num");}
};

int main() {
    std::vector vec = {0, 5, -4, 10, 500, -7, -1, 0};
    auto transformed = std::views::transform(vec, [](const auto &elem) { return Number{std::abs(elem)}; });

    // получаем итератор
    auto it = transformed.begin();
    // получили первый элемент нового диапазона с помощью функтора
    auto elem0 = *it;
    std::println("Elem0 {}", elem0.i);
    // получили второй элемент нового диапазона с помощью функтора
    auto elem1 = *(++it);
    std::println("Elem1 {}", elem1.i);
}