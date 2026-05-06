#include <iostream>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <vector>

namespace my_views {

template <std::ranges::view V>
class cycle_view: public std::ranges::view_interface<cycle_view<V>> {
private:
    V base_;

public:
    cycle_view() = default;

    constexpr explicit cycle_view(V base)
        : base_(std::move(base)) {
    }

    // Итератор для cycle_view
    class iterator {
        using Base = std::ranges::iterator_t<V>;
        using Sentinel = std::ranges::sentinel_t<V>;
        Base current_{};
        Base begin_{};
        Sentinel end_{};
        bool is_end_{false};

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::ranges::range_value_t<V>;
        using difference_type = std::ranges::range_difference_t<V>;

        iterator() = default;

        constexpr iterator(Base current, Base begin, Sentinel end)
            : current_(current)
            , begin_(begin)
            , end_(end) {
        }

        constexpr auto operator*() const -> std::ranges::range_reference_t<V> {
            return *current_;
        }

        constexpr iterator& operator++() {
            if (++current_ == end_) {
                current_ = begin_;
            }
            return *this;
        }

        constexpr iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        // Для bidirectional ranges
        constexpr iterator& operator--() requires std::ranges::bidirectional_range<V>
        {
            if (current_ == begin_) {
                current_ = end_;
            }
            --current_;
            return *this;
        }

        constexpr iterator operator--(int) requires std::ranges::bidirectional_range<V>
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const {
            return current_ == other.current_;
        }

        constexpr bool operator==(std::default_sentinel_t) const {
            return false;
        }
    };

    constexpr auto begin() {
        return iterator(std::ranges::begin(base_), std::ranges::begin(base_),
                        std::ranges::end(base_));
    }

    constexpr auto end() const {
        return std::unreachable_sentinel;
    }
};

// Адаптер для создания cycle_view
struct CycleAdapter
    : public std::ranges::range_adaptor_closure<CycleAdapter> // Наследование!
{
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R&& r) const {
        return cycle_view(std::views::all(std::forward<R>(r)));
    }
};

inline constexpr auto cycle = CycleAdapter{};

} // namespace my_views

// Специализация enable_borrowed_range (без изменений)
template <std::ranges::view V>
inline constexpr bool std::ranges::enable_borrowed_range<my_views::cycle_view<V>> =
    std::ranges::enable_borrowed_range<V>;

template <auto V>
struct CustomSentinel {
    bool operator==(const auto pos) const {
        return *pos == V;
    }
};

int main() {
    std::cout << "Pipe syntax" << std::endl;
    std::vector vec{1, 2, 3};
    for (int x : vec | my_views::cycle | std::views::take(5)) {
        std::cout << x << " "; // Теперь выведет 1 2 3 1 2
    }
    std::cout << std::endl;
    std::cout << "Functional syntax" << std::endl;
    for (int x : my_views::cycle(vec) | std::views::take(10)) {
        std::cout << x << " "; // Теперь выведет 1 2 3 1 2
    }
    std::cout << std::endl;
    std::cout << "Input range with custom sentinel" << std::endl;
    std::vector<int> vec1{1, 2, 3, 4, 5, 6, 7};
    auto sr = std::ranges::subrange(vec1.begin() + 1, CustomSentinel<5>{});
    for (int x : my_views::cycle(sr) | std::views::take(10)) {
        std::cout << x << " "; // Теперь выведет 1 2 3 1 2
    }
}
