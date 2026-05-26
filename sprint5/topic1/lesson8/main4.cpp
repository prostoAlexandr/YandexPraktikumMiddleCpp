#include <optional>
#include <print>
#include <ranges>
#include <set>
#include <vector>

struct Letter {
    std::optional<std::string> GetRecipientName() const { return _Name; }
    // ...
    std::optional<std::string> _Name;
};
std::vector<Letter> letters = {{"Name1"}, {"Name2"}, {std::nullopt}, {"Name1"}, {"Name3"}};

using namespace std;
int main() {
    // clang-format off
    auto uniqueNames =
    letters
        | views::transform(&Letter::GetRecipientName)
        // only in c++26
        | views::join
        | ranges::to<std::set>()
        | ranges::to<std::vector>();
    // clang-format on

    std::println("uniqueNames size is {}", uniqueNames.size());
    std::println("uniqueNames = {}", uniqueNames);
}