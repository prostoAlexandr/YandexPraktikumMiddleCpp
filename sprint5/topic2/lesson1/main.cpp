#include <functional>
#include <print>
#include <string>
#include <unordered_map>

struct Response {
    std::string data;
};

struct Request {
    std::string data;
    Request() = default;
    Request(Request &&other) = default;
    Request(const Request &other) = delete;
    Response operator()() {
        std::println("Process request");
        return {.data = "Result"};
    }
};

int main() {
    // Контейнер для хранения и нужно реализовать
    std::unordered_map<std::string, std::move_only_function<Response()>> requests;
    // Код для проверки
    Request r1;
    requests["db1"] = std::move(r1);  // Добавить свой вариант
    std::println("{}", requests.at("db1")().data);
}