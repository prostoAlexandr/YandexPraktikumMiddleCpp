#include <atomic>

class Mutex {
    // здесь ваш код
    std::atomic<bool> lock{false};
public:
    // определите конструктор
    void lock() noexcept {
        while (m_.exchange(true)) {
            m_.wait(true);
        }
    }
    bool try_lock() noexcept {
        // здесь ваш код
        return !lock.exchange(true);
    }
    void unlock() noexcept {
        // здесь ваш код
        lock.notify_one();
    }
};