#include <atomic>

class Spinlock {
    // здесь ваш код
    std::atomic<bool> lock;

public:
    // определите конструктор

    bool try_lock() noexcept {
        // здесь ваш код
        return !lock.exchange(true);
    }

    void lock() noexcept {
        // здесь ваш код
        while (lock.exchange(true));
    }

    void unlock() noexcept {
        // здесь ваш код
        lock = false;
    }
};