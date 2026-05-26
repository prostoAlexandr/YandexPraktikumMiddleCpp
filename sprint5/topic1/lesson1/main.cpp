#include <exception>
#include <ranges>
#include <stack>

enum class Operator { Plus, Minus };
enum class Bracket { Open, Close };

struct Number {
    int value;
};

template <class... Ts>
struct Multilambda : Ts... {
    using Ts::operator()...;
};

namespace rng = std::ranges;
namespace views = std::ranges::views;

using Token = std::variant<Number, Operator, Bracket>;

std::stack<Token> tokens_;

void ProcessNext() {
    if (tokens_.size() < 3) {
        throw std::runtime_error("Not enough tokens for a complex token");
    }

    auto lhs = tokens_.top();
    tokens_.pop();
    auto op = tokens_.top();
    tokens_.pop();
    auto rhs = tokens_.top();
    tokens_.pop();

    Token &&res =
        std::visit(Multilambda{
                       [](const Number &lhs, const Operator &op, const Number &rhs) -> Token {
                           switch (op) {
                           case Operator::Plus:
                               return Number{lhs.value + rhs.value};
                           case Operator::Minus:
                               return Number{lhs.value + rhs.value};
                           default: {
                               throw std::runtime_error("Unknown operator");
                           }
                           }
                       },
                       [](auto &&, auto &&, auto &&)->Token { throw std::runtime_error("Invalid complex token structure"); }},
                   lhs, op, rhs);
    tokens_.push(res);
}

int main() {
    while (tokens_.size() > 1) {
        ProcessNext();
    }
}