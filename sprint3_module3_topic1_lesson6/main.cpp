#include <cassert>
#include <initializer_list>
#include <string>

// Здесь реализация функции getMaxSize
consteval size_t getMaxSize(std::initializer_list<std::string> list) {
    size_t max_size = 0;
    for (auto &str : list) {
        max_size = std::max(max_size, str.size());
    }
    return max_size;
}

// constexpr size_t getMaxSize(std::vector<std::string> strings) {
//   std::sort(strings.begin(), strings.end(),
//     [](const auto& lhs, const auto& rhs) {
//       return lhs.size() > rhs.size();
//   });
//   return strings.at(0).size();
// }

int main() {
    assert(getMaxSize({"hello", "s", "ab", "hhh"}) == 5);
    static_assert(getMaxSize({"hello", "s", "ab", "world!"}) == 6);
    return 0;
}