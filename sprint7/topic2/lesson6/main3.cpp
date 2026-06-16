#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

// Генератор принимает входные значения (Push = int) и возвращает квадраты (Yield = int)
// this_coro::initial — приостанавливает генератор сразу и ждёт первого входного значения
boost::cobalt::generator<int, int> square_generator() {
    auto x = co_await boost::cobalt::this_coro::initial;
    while (x != 0) {
        x = co_yield x * x;  // возвращает квадрат, принимает новое значение
    }
    co_return -1;
} 

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    auto gen = square_generator();// ← создаётся, но СРАЗУ ждёт через `initial`
    std::println("Result: {}", co_await gen(4));
    std::println("Result: {}", co_await gen(5));
    std::println("Result: {}", co_await gen(0));
}