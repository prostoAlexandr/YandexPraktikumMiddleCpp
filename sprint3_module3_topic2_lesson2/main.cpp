#include <cassert>
#include <cstddef>
#include <iterator>
#include <optional>
#include <vector>

template <class Container, class T>
std::optional<size_t> myfind(const Container &c, const T &t) {
    for (size_t i = 0; i < c.size(); ++i) {
        if (c[i] == t) {
            return i;
        }
    }
    return std::nullopt;
}

template <typename Iter, class T>
std::optional<size_t> myfind(Iter begin, Iter end, const T &t) {
    Iter it = begin;
    while (it != end) {
        if (*it == t)
        {
            return std::distance(begin, it);
        }
        it = std::next(it);
    }
    return std::nullopt;
}

int main() {
    std::vector test = {1, 2, 3, 4, 5};
    assert(myfind(test, 3).value() == 2);
    assert(myfind(test.begin(), test.end(), 4).value() == 3);
}