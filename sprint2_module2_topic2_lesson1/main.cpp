#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <utility>
class Logger {
public:
    Logger(std::ostream &stream, const std::string &separator = ", ") : _separator(separator), _stream(stream) {}

    // Здесь ваш код
    template <typename... Args>
    void log(Args &&...args) {
        using namespace std::chrono;

        std::tm tm{};
        std::time_t now_time = system_clock::to_time_t(system_clock::now());
        localtime_r(&now_time, &tm);

        _stream << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
        do_log(std::forward<Args>(args)...);
    }

private:
    const std::string _separator;
    std::ostream &_stream;

    template <typename T, typename... Args>
    void do_log(T &&t, Args &&...args) {
        _stream << t << _separator;
        do_log(std::forward<Args>(args)...);
    }

    template <typename T>
    void do_log(T &&t) {
        _stream << t << std::endl;
    }
};

int main() {
    Logger logger(std::cout, ", ");  // Разделитель между аргументами

    logger.log("User logged in", 42, 3.14, "Hello, world!");
    // Пример вывода:
    // [2023-10-05 12:34:56] User logged in, 42, 3.14, Hello, world!

    logger.log("Processing data", 100, 200, 300);
    // Пример вывода:
    // [2023-10-05 12:34:57] Processing data, 100, 200, 300

    return 0;
}