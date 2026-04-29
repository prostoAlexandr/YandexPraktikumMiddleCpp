#include <iostream>
#include <iterator>
#include <print>
int main() {
    std::istream_iterator<int> iter(std::cin);
    int sum = 0;
    while (iter != std::istream_iterator<int>()) {
        sum += *iter;
        ++iter;
    }
    std::println("{}", sum);
}