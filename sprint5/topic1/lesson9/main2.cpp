#include <algorithm>
#include <cctype>
#include <deque>
#include <iostream>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <variant>

// Типы токенов
enum class Operator { Plus, Minus };
enum class Bracket { Open, Close };

struct Number {
    int value;
    Number operator+(Number other) { return Number(value + other.value); }
    Number operator-(Number other) { return Number(value - other.value); }
    Number operator+=(Number other) { return *this + other; }
    Number operator-=(Number other) { return *this - other; }
};

namespace rng = std::ranges;
namespace views = std::ranges::views;

using Token = std::variant<Number, Operator, Bracket>;

// Обёртка над deque<Token>
class TokenStream {
public:
    explicit TokenStream(std::deque<Token> tokens) : tokens_(std::move(tokens)) {}

    bool empty() const { return tokens_.empty(); }

    Number NextNumber() {
        if (tokens_.empty() || !std::holds_alternative<Number>(tokens_.front())) {
            throw std::runtime_error("Expected number");
        }
        Number n = std::get<Number>(tokens_.front());
        tokens_.pop_front();
        return n;
    }

    Operator NextOp() {
        if (tokens_.empty() || !std::holds_alternative<Operator>(tokens_.front())) {
            throw std::runtime_error("Expected operator");
        }
        Operator op = std::get<Operator>(tokens_.front());
        tokens_.pop_front();
        return op;
    }

    bool IsNextBracketOpen() const {
        return !tokens_.empty() && std::holds_alternative<Bracket>(tokens_.front()) &&
               std::get<Bracket>(tokens_.front()) == Bracket::Open;
    }

    bool IsNextBracketClose() const {
        return !tokens_.empty() && std::holds_alternative<Bracket>(tokens_.front()) &&
               std::get<Bracket>(tokens_.front()) == Bracket::Close;
    }

    Token pop() {
        if (tokens_.empty())
            throw std::runtime_error("Unexpected end of token stream");
        Token t = tokens_.front();
        tokens_.pop_front();
        return t;
    }

    void push_front(Token t) { tokens_.push_front(std::move(t)); }

    bool HasNext() const { return !empty(); }

    Token front() const {
        if (tokens_.empty())
            throw std::runtime_error("Empty stream");
        return tokens_.front();
    }

private:
    std::deque<Token> tokens_;
};

// Парсинг строки в токены
std::deque<Token> Tokenize(std::string_view input) {

    auto is_digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };

    auto to_token = [&](auto &&chunk) -> Token {
        const std::string_view sv{chunk.begin(), chunk.end()};
        const char ch = sv.front();

        switch (ch) {
            // clang-format off
            case '+': return Operator::Plus;
            case '-': return Operator::Minus;
            case '(': return Bracket::Open;
            case ')': return Bracket::Close;
            // clang-format on
        }

        if (is_digit(ch)) {
            int value{};
            if (std::from_chars(sv.data(), sv.data() + sv.size(), value)) {
                return Number{value};
            }
            throw std::runtime_error{std::format("Invalid number: '{}'", sv)};
        }

        throw std::runtime_error{std::format("Unexpected character: {}", ch)};
    };

    /*
     * Примените pipe operator '|' и библиотеку ranges, чтобы превратить std::string_view в std::deque<Token>
     *
     * Важно: Весь код для этого уже написан, вам остаётся только правильно применить алгоритмы из <ranges>
     */

    // clang-format off
    return input
        // Убираем пробелы
        | views::filter([](auto &&chunk) { return !std::isspace(chunk); })

        | rng::to<std::string>()

        // Группируем строки в группы из одного символа или из последовательности цифр
        | views::chunk_by([&](char a, char b) { return is_digit(a) && is_digit(b); })

        // Конвертируем каждую строку в Token, а затем в std::deque
        | views::transform(to_token)

        | rng::to<std::deque>();
    // clang-format on
}

// Вычисляем выражение без скобок
Number EvaluateTokensWithoutBrackets(TokenStream ts) {
    Number result = ts.NextNumber();

    while (ts.HasNext() && !ts.IsNextBracketClose()) {
        /* Ваш код здесь */
        Operator op = ts.NextOp();
        Number token = ts.NextNumber();
        if (op == Operator::Plus) {
            result += token;
        } else {
            result -= token;
        }
    }

    return result;
}

// Главная функция обработки выражения со скобками
Number Evaluate(TokenStream ts) {
    std::deque<Token> stack;

    while (ts.HasNext()) {
        // Нашли первую закрывающую скобку - далее надо найти её пару
        //
        // Для строки "1 + 2 - (3 + 4 - (5 - 6)) + 7"
        //                             2^    1^
        // 1 - Первая закрывающая скобка
        // 2 - Её пара

        if (ts.IsNextBracketClose()) {
            ts.pop();  // Достаём ')'

            // Вытаскиваем всё выражение до парной скобки '('
            std::deque<Token> inner;

            /* Ваш код здесь */
            while (!stack.empty()) {
                auto next = stack.back();
                stack.pop_back();
                if (std::holds_alternative<Bracket>(next) && std::get<Bracket>(next) == Bracket::Open) {
                    break;
                }
                inner.push_front(std::move(next));
            }

            // Формируем новый TokenStream без скобок, который можно вычислить
            Number reduced = EvaluateTokensWithoutBrackets(TokenStream{std::move(inner)});
            stack.emplace_back(std::move(reduced));
        } else {
            stack.push_back(ts.pop());
        }
    }

    return EvaluateTokensWithoutBrackets(TokenStream{std::move(stack)});
}

int main() {
    std::string input = "1 + 2 - (3 + 4 - (5 - 6)) + 7";

    try {
        auto tokens = Tokenize(input);

        Number result = Evaluate(TokenStream{std::move(tokens)});
        std::println("Result = {}", result.value);
    } catch (const std::exception &ex) {
        std::println(std::cerr, "Error = {}", ex.what());
    }

    return 0;
}