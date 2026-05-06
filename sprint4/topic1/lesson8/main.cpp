#include <iterator>
#include <print>
#include <random>
#include <ranges>

namespace views {

template <std::integral IntType>
class random_view : public std::ranges::view_interface<random_view<IntType>> {
public:
    random_view() = default;

    // здесь ваш параметризированный конструктор отображения
    constexpr random_view(IntType start, IntType finish, IntType seed = std::random_device{}())
        : _start(start), _finish(finish), _seed(seed) {}

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = IntType;
        using difference_type = std::ptrdiff_t;
        using pointer = const IntType *;
        using reference = const IntType &;

        iterator() = default;

        // здесь ваш параметризированный конструктор итератора
        constexpr iterator(IntType start, IntType finish, IntType seed) : _gen(seed), _dist(start, finish) {
            _val = _dist(_gen);
        }

        value_type operator*() const {
            // ваш код здесь
            return _val;
        }

        iterator &operator++() {
            // ваш код здесь
            _val = _dist(_gen);
            return *this;
        }

        iterator operator++(int) {
            // ваш код здесь
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr bool operator==(const iterator &other) const { return this == &other; }

        constexpr bool operator==(std::unreachable_sentinel_t) const { return false; }

    private:
        // здесь ваше объявление полей класса итератора
        value_type _val;
        std::mt19937 _gen;
        std::uniform_int_distribution<> _dist;
    };

    constexpr auto begin() { return iterator(_start, _finish, _seed); }
    constexpr auto end() const {
        // ваш код здесь
        return std::unreachable_sentinel;
    }

private:
    // здесь ваше объявление полей класса отображения
    IntType _start;
    IntType _finish;
    IntType _seed;
};

// Определите фабричную функцию random для создания отображения по аргументам
template <std::integral T>
inline auto random(T a, T b, T seed = std::random_device{}()) {
    // ваш код здесь
    return random_view(a, b, seed);
}

// Определите ниже перегрузку оператора | для поддержки pipe-синтаксиса
// Перегрузка оператора | для поддержки pipe-синтаксиса
template <std::integral IntType>
inline auto operator|(auto &&, const random_view<IntType> &ra) {
    return ra;
}

}  // namespace views

int main() {
    std::println("One sided pipe syntax");
    for (int num : views::random(1, 100) | std::views::take(5)) {
        std::print("{} ", num);
    }
    std::println();

    std::println("Two sided pipe syntax with fixed seed");
    for (int num : std::views::iota(0) | views::random(1, 100, 0) | std::views::take(5)) {
        std::print("{} ", num);
    }
    std::println();

    std::println("Using direct view");
    auto even_rnd =
        views::random_view(1, 100) | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::take(5);

    for (int num : even_rnd) {
        std::print("{} ", num);
    }
    std::println();

    std::println("Using direct view with fixed seed");
    for (int num : views::random_view(1, 100, 42) | std::views::take(5)) {
        std::print("{} ", num);
    }
    std::println();

    std::println("Check same sequence as before");
    for (int num : views::random_view(1, 100, 42) | std::views::take(5)) {
        std::print("{} ", num);
    }
    std::println();
}