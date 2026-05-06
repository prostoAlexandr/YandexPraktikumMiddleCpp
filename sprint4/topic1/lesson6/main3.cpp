#include <algorithm>
#include <print>
#include <ranges>
#include <string>
#include <vector>

struct Transaction {
    double amount;
    std::string category;
};

auto get_top_transactions(std::vector<Transaction> &transactions, const std::string &target_category) {
    std::ranges::sort(transactions, std::ranges::greater{}, &Transaction::amount);

    auto top_transactions =
        transactions | std::views::filter([target_category](const auto &t) { return t.category == target_category; }) |
        std::views::transform([](const auto &t) { return Transaction{t.amount * 100, t.category}; }) |
        std::views::take(3);
    return top_transactions;
}

int main() {
    std::vector<Transaction> transactions = {{1, "sell"},  {2, "buy"},   {3, "mort"},  {4, "sell"}, {5, "buy"},
                                             {6, "sell"},  {7, "sell"},  {8, "buy"},   {9, "sell"}, {10, "sell"},
                                             {11, "buy"},  {12, "sell"}, {13, "sell"}, {14, "buy"}, {15, "sell"},
                                             {16, "sell"}, {17, "buy"},  {18, "mort"}};
    for (const auto &tr : get_top_transactions(transactions, "buy")) {
        std::println("amount: {}, cat: {}", tr.amount, tr.category);
    }
    for (const auto &tr : get_top_transactions(transactions, "mort")) {
        std::println("amount: {}, cat: {}", tr.amount, tr.category);
    }
}