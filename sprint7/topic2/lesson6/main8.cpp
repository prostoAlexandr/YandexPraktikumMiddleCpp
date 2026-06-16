#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

boost::cobalt::promise<int> task(int id, int delay_ms) {
    if (delay_ms > 1000) {
        throw std::runtime_error("Too slow!");
    }
    boost::asio::steady_timer t{
        co_await boost::cobalt::this_coro::executor,
        std::chrono::milliseconds(delay_ms)
    };
    co_await t.async_wait(boost::cobalt::use_op);
    co_return id;
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    // gather ждёт все задачи и НИКОГДА не бросает исключение.
    // Вместо этого возвращает optional для каждой задачи.
    auto results = co_await boost::cobalt::gather(
        task(1, 200),
        task(2, 1500),
        task(3, 300)
    );

    if (std::get<0>(results).has_value())
        std::println("Task 1: {}", *std::get<0>(results));
    else
        std::println("Task 1 failed");

    if (std::get<1>(results).has_value())
        std::println("Task 2: {}", *std::get<1>(results));
    else
        std::println("Task 2 failed");

    if (std::get<2>(results).has_value())
        std::println("Task 3: {}", *std::get<2>(results));
    else
        std::println("Task 3 failed");

    co_return 0;
} 