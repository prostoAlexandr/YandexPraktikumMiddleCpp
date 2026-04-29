#include <print>
#include <string>
#include <unordered_map>

void test1() {
    std::unordered_map<int, std::string> players;
    players.emplace(1, "John");
    players.emplace(2, "Michael");
    players.emplace(3, "Todd");
    players.emplace(4, "Chris");

    for (const auto &[id, name] : players) {
        std::println("id={}, name={}", id, name);
    }
}

void test2() {
    for (const auto &[id, name] : std::unordered_map<int, std::string>{{1, "John"}, {2, "Michael"}, {3, "Todd"}, {4, "Chris"}}) {
        std::println("id={}, name={}", id, name);
    }
}

int main() {
    test1();
    test2();
}