#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <ranges>
#include <print>

struct Transaction {
    double amount;
    std::string category;
};

Transaction get_top_transactions(const std::vector<Transaction> &transactions) {
    return *std::ranges::max_element(transactions, {}, &Transaction::amount);
}

int main()
{
    auto top = get_top_transactions({{10, "sell"}, {11, "buy"}, {100500, "mortgage"}});
    std::println("Top: {}", top.category);
}