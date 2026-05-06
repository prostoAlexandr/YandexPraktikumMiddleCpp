#include <print>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/filter.hpp>
#include <string>
#include <string_view>

using namespace ranges;
auto get_error_list(const std::string &log_text) {
    return log_text | views::split_when([](char c) { return c == '\n'; }) | views::tail |
           views::transform([](auto &&range) {
               auto v = views::common(range);
               return std::string(v.begin(), v.end());
           }) |
           views::chunk_by([](const std::string& a, const std::string_view& b) {
               return a.substr(1, 4) == b.substr(1, 4);
           }) |
           views::filter([](const auto &range) {
               return (*range.begin()).starts_with("[ERROR]");
           });
}

int main() {
    std::string log_text = "Started logging...\n"
                           "[ERROR] Connection failed\n"
                           "[ERROR] Timeout reached\n"
                           "[INFO] Retrying...\n"
                           "[WARN] High latency\n"
                           "[INFO] Connection established\n"
                           "[INFO] Performing request handling...\n"
                           "[ERROR] DB error\n"
                           "[ERROR] Could not connect to db using following credantials";
    auto list = get_error_list(log_text);
    for (const auto &chunk : list) {
        for (const auto &val : chunk) {
            std::print("{} ", val);
        }
        std::println("");
    }
}