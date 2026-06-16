#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

boost::cobalt::promise<int> delayed_value(int ms) {
    std::println("Promise started");
    boost::asio::steady_timer timer{
        // Получаем executor текущей сопрограммы — он управляет выполнением асинхронных операций
        co_await boost::cobalt::this_coro::executor,
        std::chrono::milliseconds(ms)
    };
    // Асинхронно ждём срабатывания таймера
    co_await timer.async_wait(boost::cobalt::use_op);
    std::println("Promise done");
    co_return 42;
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    std::println("Calling promise...");
    auto p = delayed_value(1000);
    std::println("Promise called, not yet awaited");
    int result = co_await p;
    std::println("Result: {}", result);
    co_return 0;
}