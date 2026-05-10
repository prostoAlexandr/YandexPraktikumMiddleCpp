#include <ranges>
#include <string>
#include <vector>

int main() {
    std::string text = "C++ Ranges are powerful";
    auto words = text | std::views::split(' ') |
                 std::views::transform([](auto &&range) { return std::ranges::to<std::string>(); }) |
                 std::ranges::to<std::vector>();
}