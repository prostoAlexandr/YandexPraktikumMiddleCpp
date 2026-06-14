#include <atomic>

class RWLock {
    // ваш код здесь
    std::atomic<int> counter;

public:
    void read_lock() {
        while (true) {
            int prev_readers = std::atomic_load(&counter);
            if (prev_readers >= 0) {
                // ваш код здесь
                int readers = prev_readers + 1;
                if (std::atomic_compare_exchange_strong(&counter, &prev_readers, readers)) {
                    return;
                }
            } else {
                // ваш код здесь
                std::atomic_wait(&counter, 0);
            }
        }
    }

    void read_unlock() {
        // ваш код здесь
        std::atomic_fetch_sub(&counter, 1);
    }

    void write_lock() {
        // ваш код здесь
        while (std::atomic_exchange(&counter, -1) > 0)
            ;
    }

    void write_unlock() {
        // ваш код здесь
        std::atomic_exchange(&counter, 0);
        // Возможно будет лучше реализация на отдельном std::atomic<bool> can_read
        std::atomic_notify_all(&counter);
    }
};


class RWLock {
    std::atomic<int> readers = 0;
    std::atomic<bool> can_read = false;
    static constexpr int MIN_INT = std::numeric_limits<int>::min();

public:
    void read_lock() {
        while (true)
        {
            int prev_readers = std::atomic_load(&readers);
            if (prev_readers != MIN_INT)
            {
                int new_readers = prev_readers + 1;
                if (std::atomic_compare_exchange_weak(&readers, &prev_readers, new_readers))
                {
                    return;
                }
            } else {
                std::atomic_wait(&can_read, false);
            }
        }
    }

    void read_unlock() {
        std::atomic_fetch_sub(&readers, 1);
    }

    void write_lock() {
        int expected = 0;
        while(!std::atomic_compare_exchange_weak(&readers, &expected, MIN_INT)){
            expected = 0;
        }
        std::atomic_store(&can_read, false);
    }

    void write_unlock() {
        std::atomic_store(&readers, 0);
        std::atomic_store(&can_read, true);
        std::atomic_notify_all(&can_read);
    }
};