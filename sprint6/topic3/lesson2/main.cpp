#include <condition_variable>
#include <mutex>
#include <thread>
#include <print>

class simple_latch {
public:
    explicit simple_latch(size_t count) : count(count) {}
    void arrive_and_wait() {
        std::unique_lock lk(mut);
        --count;
        if (count > 0) {
            cv.wait(lk, [this] { return count <= 0; });
            return;
        }
        lk.unlock();
        cv.notify_all();
    }

private:
    size_t count;
    /* Ваш код здесь */
    std::mutex mut;
    std::condition_variable cv;
};

int main() {
    using namespace std::chrono_literals;
    constexpr size_t NUM_SERVICES = 3;
    simple_latch init_completed(NUM_SERVICES);

    std::jthread producer([&] {
        // инициализация
        std::println("Producer arrived");
        init_completed.arrive_and_wait();
        // работа
        std::println("Producer works");
    });

    std::jthread consumer([&] {
        // инициализация
        std::this_thread::sleep_for(5ms);
        std::println("Consumer arrived");
        init_completed.arrive_and_wait();
        // работа
        std::println("Consumer works");
    });

    std::jthread logger([&] {
        // инициализация
        std::this_thread::sleep_for(10ms);
        std::println("Logger arrived");
        init_completed.arrive_and_wait();
        // работа
        std::println("Logger works");
    });

    return 0;
}