#include <expected>
#include <print>
#include <string>
#include <vector>

enum class LoginError { InvalidPassword, UserNotFound, ConnectionFailed };

struct User {
    std::string display_name;
};

std::expected<User, LoginError> TryLogin(std::string_view username, std::string_view password);
std::string DescribeError(LoginError err);

std::vector<std::pair<std::string, std::string>> credentials{{"username", "password"}};
std::vector<std::string> loginMessages;

int main() {
    for (const auto &[username, password] : credentials) {
        // Ваш код здесь
        auto result = TryLogin(username, password)
                          .and_then([](User &&usr) -> std::expected<std::string, LoginError> {
                              return "Welcome, " + usr.display_name;
                          })
                          .or_else([](LoginError &&error) -> std::expected<std::string, LoginError> {
                              return std::format("Login failed: {}", DescribeError(error));
                          })
                          .value();
        loginMessages.push_back(std::move(result));
    }
}