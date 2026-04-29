#include <functional>
#include <vector>
template <typename Container, typename = decltype(std::declval<Container const &>().size())>
auto f(Container const &cont) {
    return cont.size();
}

int main() {
    auto test = f(std::vector<int>{1, 3, 4});
    // auto test1 = f(2); // Not compiling
}