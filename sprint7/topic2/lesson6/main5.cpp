#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

using namespace std::chrono_literals;
// detached — фоновая задача, которая запускается сразу и не требует co_await
// Полезно для логирования, мониторинга и т. п.
boost::cobalt::detached heartbeat_logger() {
    for (int i = 0; i < 3; ++i) {
        std::println("Heartbeat {}", i);
        // Получаем executor текущей сопрограммы — он управляет выполнением асинхронных операций
        boost::asio::steady_timer timer{
            co_await boost::cobalt::this_coro::executor,
            1s
        };
        // Асинхронно ждём срабатывания таймера
        co_await timer.async_wait(boost::cobalt::use_op);
    }
    std::println("Heartbeat finished");
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    heartbeat_logger(); // Запускается немедленно и "уходит в фон"
    std::println("Main continues immediately");
    // Ждём 4 секунды, чтобы фоновая задача успела завершиться
    boost::asio::steady_timer timer{
        co_await boost::cobalt::this_coro::executor,
        4s
    };
    co_await timer.async_wait(boost::cobalt::use_op);
    co_return 0;
}