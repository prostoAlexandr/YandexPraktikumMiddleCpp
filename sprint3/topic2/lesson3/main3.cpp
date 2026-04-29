#include <map>
#include <print>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

struct MyKey {
    int id;
    std::string textId;
    MyKey(int _id) : id(_id), textId(std::format("dev-{}", _id)) {}
};

struct TransparentCmp {
    using is_transparent = void;

    static int extract(int x) { return x; }

    static int extract(const MyKey& k) { return k.id; }

    static int extract(std::string_view sv) {
        int x{};
        std::from_chars(sv.data() + sv.find('-') + 1,
                        sv.data() + sv.size(), x);
        return x;
    }

    template<typename L, typename R>
    bool operator()(const L& lhs, const R& rhs) const {
        return extract(lhs) < extract(rhs);
    }
};

int main() {
    // Создайте std::map и вставьте 100'000 элементов, используя 4 разных способа
    std::map<MyKey, int, TransparentCmp> configMap;

    for (int i = 0; i < 25'000; ++i) {
        configMap.insert({MyKey(i), i});
    }

    for (int i = 25'000; i < 50'000; ++i) {
        configMap.emplace(MyKey(i), i);
    }

    for (int i = 50'000; i < 75'000; ++i) {
        configMap.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(i));
    }

    for (int i = 75'000; i < 100'000; ++i) {
        configMap.try_emplace(i, i);
    }

    auto it = configMap.find(150);
    if (it != configMap.end()) {
        std::println("Found by int: {} -> {}", it->first.id, it->second);
    }

    // Проверка по string_view
    std::string_view target = "dev-56789";
    std::println("Contains key '{}': {}", target, configMap.contains(target));

    // Подсчет по string_view
    std::println("Count of key '{}': {}", target, configMap.count(target));
}