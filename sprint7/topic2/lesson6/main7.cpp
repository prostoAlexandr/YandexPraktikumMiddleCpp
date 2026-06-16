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
    try {
        // join ждёт ВСЕ задачи, но при первой ошибке пробрасывает исключение
        auto [a, b, c] = co_await boost::cobalt::join(
            task(1, 200),
            task(2, 1500), // Эта задача выбросит исключение
            task(3, 300)
        );
        std::println("Results: {}, {}, {}", a, b, c);
    } catch (const std::exception& e) {
        std::println("join failed: {}", e.what());
    }
    co_return 0;
}