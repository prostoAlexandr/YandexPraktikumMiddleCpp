#include <cassert>
#include <optional>
#include <string>

struct User {
    std::string name;
};

enum class OrderBy {
    Age = 1,
    Weight = 2,
};

constexpr std::string to_string(OrderBy orderBy) {
    switch (orderBy) {
    case OrderBy::Age:
        return "Age";
    case OrderBy::Weight:
        return "Weight";
    }
}

struct Query {
    User user;
    std::optional<OrderBy> orderBy;
};

constexpr std::string Serialize(const Query &query) {
    std::string result("SELECT * FROM Users WHERE Name == \"");
    result += query.user.name + "\"";
    if (query.orderBy)
    {
        result += " ORDER BY " + to_string(query.orderBy.value());
    }
    result.push_back(';');
    return result;
}

int main() {
    static_assert(Serialize({{"Mary"}, {}}) == "SELECT * FROM Users WHERE Name == \"Mary\";");
    static_assert(Serialize({{"John"}, {OrderBy::Age}}) == "SELECT * FROM Users WHERE Name == \"John\" ORDER BY Age;");
    return 0;
}