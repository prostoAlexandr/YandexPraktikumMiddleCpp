#include <optional>
#include <string>
#include <vector>

struct Letter {
    std::optional<std::string> GetRecipientName() const;
    // ...
};
std::vector<Letter> letters;

template <typename T, typename F>
auto operator>>=(std::optional<T> &&opt, F &&callable) {
    return std::move(opt).or_else(callable);
}

template <typename T, typename F>
auto operator|(std::optional<T> &&opt, F &&callable) {
    return std::move(opt).transform(callable);
}

int main() {
    std::vector<std::string> labeledRecipients;
    for (const Letter &letter : letters) {

        // Ваш код здесь
        auto label = letter.GetRecipientName() | [](const auto rec) { return "To: " + rec; } >>=
            [] { return std::optional{std::string{"To: [unknown]"}}; };

        labeledRecipients.push_back(label.value());
    }
}