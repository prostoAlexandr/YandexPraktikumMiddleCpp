#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/join.hpp>
#include <boost/cobalt/main.hpp>
#include <boost/variant2/variant.hpp>
#include <optional>
#include <print>
#include <string>

using namespace std::chrono_literals;

boost::cobalt::promise<std::string> fetch_candles() {
    boost::asio::steady_timer timer{co_await boost::cobalt::this_coro::executor, 150ms};
    co_await timer.async_wait(boost::cobalt::use_op);
    co_return "CANDLES: [10 items]";
}

boost::cobalt::promise<std::string> fetch_orderbook(int depth) {
    boost::asio::steady_timer timer{co_await boost::cobalt::this_coro::executor, 150ms};
    co_await timer.async_wait(boost::cobalt::use_op);
    co_return std::format("ORDERBOOK depth={}: ready", depth);
}

boost::cobalt::task<void> market_data_logger(boost::cobalt::channel<std::string> &ch) {
    while (ch.is_open()) {
        try {
            auto data = co_await ch.read();  // Блокирует, пока нет данных
            std::println("Market {}", data);
        } catch (boost::system::system_error &e) {
            break;
        }
    }
    co_return;
}

boost::cobalt::task<void> signal_handler(boost::cobalt::channel<std::string> &signal_ch,
                                         boost::cobalt::channel<std::string> &market_data_ch) {
    // your code here
    std::string prev_sig;
    while (signal_ch.is_open()) {
        try {
            auto sig = co_await signal_ch.read();
            int depth = 0;
            if (sig == prev_sig) {
                continue;
            } else if (sig == "SELL") {
                depth = 10;
            } else if (sig == "BUY") {
                depth = 5;
            } else {
                continue;
            }
            prev_sig = sig;
            auto fetch_result =
                co_await boost::cobalt::left_race(fetch_orderbook(depth), fetch_candles());
            co_await market_data_ch.write(
                boost::variant2::visit([](const auto &v) { return v; }, fetch_result));
        } catch (...) {
            break;
        }
    }
    market_data_ch.close();
    co_return;
}

// ─── Генератор сигналов ─────────────────────────────────────────────────────
boost::cobalt::task<void> signal_producer(boost::cobalt::channel<std::string> &ch) {

    boost::asio::steady_timer timer{co_await boost::cobalt::this_coro::executor, 300ms};
    std::vector<std::string> signals = {"SELL", "BUY", "SELL", "BUY", "BUY",  "HOLD", "SELL",
                                        "SELL", "BUY", "SELL", "BUY", "SELL", "BUY"};
    for (const auto &sig : signals) {
        co_await ch.write(sig);
        co_await timer.async_wait(boost::cobalt::use_op);
    }
    ch.close();
    co_return;
}

boost::cobalt::main co_main(int, char *[]) {
    boost::cobalt::channel<std::string> signal_ch;
    boost::cobalt::channel<std::string> market_data_ch;
    // your code here
    auto sig_prod = signal_producer(signal_ch);
    auto sig_hndl = signal_handler(signal_ch, market_data_ch);
    auto mkt_dlog = market_data_logger(market_data_ch);
    co_await boost::cobalt::join(sig_prod, sig_hndl, mkt_dlog);
    co_return 0;
}