#include <array>
#include <iostream>
#include <print>
#include <string_view>
#include <tuple>
#include <type_traits>

// Фичи устройств
struct USB {};
struct Bluetooth {};
struct WiFi {};
struct FrontCamera {};
struct LTE {};

template <int Version, typename... Features>
struct DeviceConfig {
    static constexpr int version = Version;
    using FeatureList = std::tuple<Features...>;
};

// Фильтры
template <int MinVersion>
struct VersionAtLeast {
    template <typename Device>
    // Ваш код здесь
    static constexpr bool value = (Device::version >= MinVersion);
};

template <typename Feature>
struct HasFeature {
    template <typename Device>
    static constexpr bool value = []<typename... Features>(std::tuple<Features...>) {
        return (std::is_same_v<Feature, Features> || ...);
    }(typename Device::FeatureList{});
};

// Функции
template <size_t Idx, typename Pred, typename Tuple>
constexpr auto value_or_empty(const Tuple &t) {
    // Ваш код здесь
    using T = std::tuple_element_t<Idx, Tuple>;
    if constexpr (Pred::template value<T>) {
        return std::tuple{std::get<Idx>(t)};
    } else {
        return std::tuple{};
    }
}

template <typename Pred, typename Tuple>
constexpr auto filter_tuple(const Tuple &t) {
    // Ваш код здесь
    constexpr auto N = std::tuple_size_v<Tuple>;

    return [&]<size_t... Idxs>(std::index_sequence<Idxs...>) {
        return std::tuple_cat(value_or_empty<Idxs, Pred>(t)...);
    }(std::make_index_sequence<N>{});
}

template <int Idx, typename Tuple>
auto print_one_device(const Tuple &) {
    using El = std::tuple_element_t<Idx, Tuple>;
    constexpr auto ver = El::version;
    constexpr auto size = std::tuple_size_v<typename El::FeatureList>;

    std::println(" - Device v{} with {} features", ver, size);
};

template <typename Tuple>
void print_devices_tuple(const Tuple &t) {
    // Ваш код здесь
    constexpr auto N = std::tuple_size_v<Tuple>;

    [&]<size_t... Idxs>(std::index_sequence<Idxs...>) {
        (print_one_device<Idxs>(t), ...);
    }(std::make_index_sequence<N>{});
}

int main() {
    using D1 = DeviceConfig<1, USB, Bluetooth>;
    using D2 = DeviceConfig<2, USB, WiFi>;
    using D3 = DeviceConfig<3, USB, WiFi, LTE>;
    using D4 = DeviceConfig<4, USB, WiFi, FrontCamera, LTE>;

    constexpr auto allDevices = std::tuple{D1{}, D2{}, D3{}, D4{}};

    // Фильтрация по версии >= 3
    constexpr auto versionFiltered = filter_tuple<VersionAtLeast<3>>(allDevices);
    print_devices_tuple(versionFiltered);
    std::println("\n\n");

    // Фильтрация из предыдущего результата — только с FrontCamera
    constexpr auto finalFiltered = filter_tuple<HasFeature<FrontCamera>>(versionFiltered);
    print_devices_tuple(finalFiltered);
}