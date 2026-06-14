#include <atomic>

std::atomic_flag shutdown = ATOMIC_FLAG_INIT;
ThreadSafeQueue tasks;

// эта функция вызывается при появлении сигнала
void signal_handler(int signal) {
    // здесь ваш код
    shutdown.test_and_set(true);
}

int main() {
    std::signal(SIGINT, signal_handler);
    std::thread worker_thread([&]() {
        while (!shutdown.test())
        {
            auto task = tasks.pop();
            task.execute();
        }
        std::println("Worker stopped!");
    });

    // fill tasks
}