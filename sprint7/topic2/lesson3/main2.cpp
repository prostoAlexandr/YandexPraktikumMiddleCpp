#include <cassert>
#include <coroutine>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

template <typename T>
class parser {

public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    parser(handle_type h) : coro(h) {}
    ~parser() {
        if (coro)
            coro.destroy();
    }

    parser(const parser &) = delete;
    parser &operator=(const parser &) = delete;

    parser(parser &&other) noexcept : coro(other.coro) { other.coro = nullptr; }

    parser &operator=(parser &&other) noexcept {
        if (this != &other) {
            if (coro)
                coro.destroy();
            coro = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }

    bool next() {
        if (!coro || coro.done())
            return false;
        coro.resume();
        return !coro.done();
    }

    T value() const { return coro.promise().value; }

private:
    handle_type coro;
};

template <class T>
struct parser<T>::promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;

    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    auto yield_value(T v) {
        value = v;
        return std::suspend_always{};
    }

    T value;
};

struct Book {
    std::string name;
    std::string author;
    double rating = 0.0;

    friend std::ostream &operator<<(std::ostream &os, const Book &b) {
        return os << "\"" << b.name << "\" by " << b.author << " [" << b.rating << "/5]";
    }
};

std::string extract_quoted_or_unquoted(const std::string &input) {
    std::string s = input;
    // Удаляем пробелы по краям
    auto start = s.find_first_not_of(" \t");
    auto end = s.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    s = s.substr(start, end - start + 1);

    // Если строка в кавычках — удаляем их
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

parser<Book> parse_books(std::istream &input) {
    std::string line;
    Book current_book;
    bool has_name = false;
    bool has_author = false;

    while (std::getline(input, line)) {
        // Удалить возможные пробелы в начале/конце
        auto start = line.find_first_not_of(" \t");
        auto end = line.find_last_not_of(" \t");
        if (start == std::string::npos)
            continue;  // пустая строка
        line = line.substr(start, end - start + 1);

        // Пропускаем пустые строки и комментарии (если есть)
        if (line.empty())
            continue;

        // Проверяем разделитель
        if (line == "---") {
            // your code here
            co_yield current_book;
            current_book = Book{};
            has_name = false;
            has_author = false;
            continue;
        }

        // Извлечение поля
        if (line.rfind("name:", 0) == 0) {
            std::string raw_value = line.substr(5);  // после "name:"
            current_book.name = extract_quoted_or_unquoted(raw_value);
            if (!current_book.name.empty())
                has_name = true;
        } else if (line.rfind("author:", 0) == 0) {
            // your code here
            std::string raw_value = line.substr(7);  // после "name:"
            current_book.author = extract_quoted_or_unquoted(raw_value);
            if (!current_book.author.empty())
                has_author = true;
        } else if (line.rfind("rating:", 0) == 0) {
            std::string raw_value = line.substr(7);  // после "rating:"
            try {
                // your code here
                current_book.rating = std::stod(raw_value);
            } catch (...) {
                current_book.rating = 0.0;  // значение по умолчанию
            }
        }
    }

    // your code here
    if (has_name || has_author) {
        co_yield current_book;
    }
}

int main() {
    std::stringstream input(R"(name: "The C++ Programming Language"
author: "Bjarne Stroustrup"
rating: 4.5
---
name: Clean Code
author: "Robert Martin"
rating: 4.8
---
name: Test
)");

    auto parser = parse_books(input);
    while (parser.next()) {
        std::cout << parser.value() << "\n";
    }

    return 0;
}
