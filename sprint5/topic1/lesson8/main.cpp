#include <optional>
#include <string>
#include <vector>

struct Letter {
    std::optional<std::string> GetRecipientName() const;
    // ...
};
std::vector<Letter> letters;

int main() {
    std::vector<std::string> labeledRecipients;
    for (const Letter &letter : letters) {

        // Ваш код здесь
        auto label = letter.GetRecipientName().transform([](const auto rec) { return "To: " + rec; }).or_else([] {
            return std::optional{std::string{"To: [unknown]"}};
        }).value();

        labeledRecipients.push_back(label);
    }
}