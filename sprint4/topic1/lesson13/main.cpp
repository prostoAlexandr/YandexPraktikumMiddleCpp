#include <generator>
#include <print>

// генерирует числа Фибоначчи, пока они помещаются в unsigned long long
std::generator<unsigned long long> fibonacci() {
    auto a = 0ull;
    auto b = 1ull;

    co_yield a;
    co_yield b;

    while (true) {
        // переполнение беззнакового целого не приводит к UB
        auto next = a + b;
        if (next < a) {  // произошло переполнение
            break;
        }
        co_yield next;
        a = b;
        b = next;
    }
}

int main() {
    namespace v = std::views;
    std::println("Первые 20 чётных чисел Фибоначчи:");
    for (auto n : fibonacci() | v::filter([](auto x) { return x % 2 == 0; }) | v::take(20)) {
        std::println("{}", n);
    }
}