#include <array>
#include <expected>
#include <print>
#include <string>
#include <string_view>

struct Config {
    std::string key;
    std::string value;
    std::array<std::byte, 1024> metadata_;
};

// Эмулируем загрузку большого файла
std::string ReadFile(std::string_view path) { return /*std::string("GameCfg:") +*/ std::string(16'384, 'A'); }

std::expected<std::string, std::string> load_file(std::string_view path) {
    // Ваш код здесь
    if (path != "config.cfg") {
        return std::unexpected("File not found");
    }
    return std::expected<std::string, std::string>{std::in_place, ReadFile(path)};
}

std::expected<Config, std::string> parse_config(std::string_view content) {
    if (!content.starts_with("GameCfg:")) {
        return std::unexpected("Invalid config");
    }
    return std::expected<Config, std::string>{std::in_place, std::string(content.substr(590, 10)), std::string(content.substr(601, 8))};
}

int main() {
    auto content_result = load_file("config.cfg");
    // Если получена ошибка, выведите её в консоль и верните 1
    if (!content_result.has_value())
    {
        std::println("{}", content_result.error());
        return 1;
    }

    auto cfg = parse_config(content_result.value());
    // Если получена ошибка, выведите её в консоль и верните 1
    if (!cfg.has_value())
    {
        std::println("{}", cfg.error());
        return 1;
    }

    // Напечатайте в консоль значения key/value из cfg
    std::println("key: {}, value: {}", cfg.value().key, cfg.value().value);
    return 0;
}