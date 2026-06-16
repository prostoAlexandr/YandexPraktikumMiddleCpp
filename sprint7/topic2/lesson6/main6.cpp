#include <print>
#include <boost/asio.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/main.hpp>
// Producer отправляет числа в канал
boost::cobalt::detached producer(boost::cobalt::channel<int>& ch) {
    for (int i = 1; i <= 3; ++i) {
        std::println("Producer: sending {}", i); // Блокируется, пока consumer не вызовет read (канал без буфера)
        co_await ch.write(i);
    }
    ch.close(); // Завершаем канал — consumer получит сигнал завершения
}

// co_main — это специальная точка входа для программ с сопрограммами в Boost.Cobalt.
// Под капотом она запускает event loop (на основе Boost.Asio) и позволяет использовать co_await в main.
boost::cobalt::main co_main(int, char*[]) {
    boost::cobalt::channel<int> ch;
    producer(ch);

    while (ch.is_open()) {
        int val = co_await ch.read(); // Блокируется, пока producer не отправит значение
        std::println("Consumer: received {}", val);
    }
    co_return 0;
}