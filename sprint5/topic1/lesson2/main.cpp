#include <cstddef>
#include <memory>
#include <tuple>

template <size_t Idx, template <typename...> typename F, typename Tuple>
constexpr auto value_of_empty(const Tuple &t) {
    using T = std::tuple_element_t<Idx, Tuple>;
    // auto val = std::get<Idx>(t);
    // if constexpr (F<decltype(val)>::value) {
    if constexpr (F<T>::value) {
        return std::tuple{std::get<Idx>(t)};
    } else {
        return std::tuple{};
    }
}

template <template <typename...> typename F, typename Tuple>
constexpr auto filter(const Tuple &t) {
    constexpr auto N = std::tuple_size_v<Tuple>;

    return [&]<size_t... Idxs>(std::index_sequence<Idxs...>) {
        // Как применить F для каждого типа нашего кортежа?
        return std::tuple_cat((value_of_empty<Idxs, F>(t), ...));
    }(std::make_index_sequence<N>{});
}

int main() {
    auto ptr = std::make_unique<int>(42);
    std::tuple t{1, 2., 'a', ptr.get()};

    auto filtered = filter<std::is_arithmetic>(t);
    // print_tuple("filtered by std::is_arithmetic", filtered);
    // tuple (filtered by std::is_arithmetic) = [ 1 2 a ]
}