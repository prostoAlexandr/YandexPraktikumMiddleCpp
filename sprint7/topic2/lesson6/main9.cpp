#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>

using namespace std::chrono_literals;

boost::cobalt::promise<std::string> fast() {
    boost::asio::steady_timer t{
        co_await boost::cobalt::this_coro::executor, 100ms
    };
    co_await t.async_wait(boost::cobalt::use_op);
    co_return "fast";
}

boost::cobalt::promise<std::string> slow() {
    boost::asio::steady_timer t{
        co_await boost::cobalt::this_coro::executor, 500ms
    };
    co_await t.async_wait(boost::cobalt::use_op);
    co_return "slow";
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    // race — возвращает результат первой завершившейся задачи (недетерминированно при равенстве)
    auto winner = co_await boost::cobalt::race(fast(), slow());
    // left_race — при одновременной готовности выбирает САМУЮ ЛЕВУЮ (первую в списке)
    auto winner_left = co_await boost::cobalt::left_race(fast(), slow());
    std::println("race winner: {}", winner.index());
    std::println("left_race winner: {}", winner_left.index());
    co_return 0;
}