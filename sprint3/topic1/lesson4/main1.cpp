#include <vector>
#include <iterator>
#include <algorithm>

template <class Container>
constexpr size_t findUniqueCount(const Container& items) {
    std::vector<typename Container::value_type> v{items.begin(), items.end()};
    std::sort(v.begin(), v.end());
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
    return v.size();
}

int main() {
    static_assert(findUniqueCount(std::vector<std::string>{"a", "c", "c", "z", "z", "a"}) == 3);
    static_assert(findUniqueCount(std::vector{1, 2, 100, 11, 22, 22, 2}) == 5);
    return 0;
}