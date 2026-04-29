#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include <string>
#include <map>
#include <print>

struct MyType {
    std::string name;
    int age;
};

int main()
{
    std::map<int, MyType> m;
    m.insert({1, {"Anna", 63}}); // worst way
    m.emplace(3, MyType{"Chris", 77}); // Fine way
    m.emplace(std::pair<int, MyType>(
        std::piecewise_construct,
        std::forward_as_tuple(2),
        std::forward_as_tuple("Boris", 44))); // better way
    m.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(5),
        std::forward_as_tuple("Evan", 19)); // more better way
    m.try_emplace(4, "Dan", 38); // The best possible way
    for (const auto& [key, val] : m)
    {
        std::println("Key: {}, Name: {}, Age: {}", key, val.name, val.age);
    }
}