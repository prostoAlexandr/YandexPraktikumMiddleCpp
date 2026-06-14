#include <chrono>
#include <thread>
#include <future>
#include <print>

using namespace std::chrono_literals;

std::int64_t selectTotalClicks() {
    std::this_thread::sleep_for(30s);  // emulates CH heavy query execution
    return 100501;  // select Sum(Clicks) from ClickHouse Where ClientId in (123987) Group BY OrderId;
}

int main() {
    auto duration = 0s;
    auto step = 5s;
    // your code here
    auto task = std::async(std::launch::async, selectTotalClicks);
    while (task.wait_for(5s) != std::future_status::ready)
    {
        std::println("Waiting for answer...");
    }
    std::println("Result is {}", task.get());
}