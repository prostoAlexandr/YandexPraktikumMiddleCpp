#include <array>
#include <print>
#include <set>
#include <string>
#include <string_view>

struct Id {
    int part1, part2, part3;
    auto operator<=>(const Id &other) const = default;
};

struct HeavyObject {
    std::array<int, 128> data{};
    Id id;
    std::string name;
    auto operator<=>(const HeavyObject &other) const { return id <=> other.id; }

    // Bullshit
    // friend auto operator<=>(std::string_view lhs, const HeavyObject &rhs) { 
    //     std::println("{}, lhs: {}, rhs.hame: {}", __PRETTY_FUNCTION__, lhs, rhs.name);
    //     return rhs.name <=> lhs; }
    // friend auto operator<=>(const HeavyObject &lhs, std::string_view rhs) { return rhs <=> lhs; }
    friend auto operator<=>(const Id &lhs, const HeavyObject &rhs) { return lhs <=> rhs.id; }
    friend auto operator<=>(const HeavyObject &lhs, const Id &rhs) { return lhs.id <=> rhs; };
};

int main() {
    /* Ваш код для std::set и вставки элементов здесь */
    std::set<HeavyObject, std::less<>> s = {
        {{}, {1, 2, 3}, "Target_GPU_Param"},
        {{}, {4, 5, 6}, "ConfigParam_A"},
        {{}, {7, 8, 9}, "ConfigParam_A"},
    };

    if (s.find(Id{7, 8, 9}) != s.end()) {
        std::println("Found element with id ID(7, 8, 9)");
    }

    // Bullshit
    // std::println("Has element with name=Target_GPU_Param: {}", s.contains(std::string_view{"Target_GPU_Param"}));

    // std::println("Num of elements with name=ConfigParam_A: {} of: {}", s.count(std::string_view{"ConfigParam_A"}), s.size());
}