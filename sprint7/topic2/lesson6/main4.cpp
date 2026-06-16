#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

// Генератор чисел Фибоначчи — eager: первое значение готовится сразу при вызове
boost::cobalt::generator<int> fibonacci_sequence() {
    int f0 = 0, f1 = 1;
    while (true) {
        co_yield f0;
        int f = f0 + f1;
        f0 = f1;
        f1 = f;
    }
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    auto gen = fibonacci_sequence();// тело НАЧИНАЕТ выполняться сразу
    std::println("Result: {}", co_await gen);
    std::println("Result: {}", co_await gen);
}