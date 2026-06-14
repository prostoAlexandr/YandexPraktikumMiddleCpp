#include <array>
#include <atomic>
#include <optional>

template <typename T, size_t N>
class SingleProducerSingleConsumerQueue {
private:
    std::array<T, N> buffer_;
    std::atomic<size_t> back_{0};
    std::atomic<size_t> front_{N - 1};

    template <typename U>
    bool Push(U &&u) {
        // Получаем текущие индексы в буфере
        size_t front = front_.load(std::memory_order_acquire);
        size_t back = back_.load(std::memory_order_relaxed);

        // Поскольку только один поток вызывает Push,
        // то после чтения back_ его значение не изменится

        // Если буфер полон, то индексы back и front одинаковые.
        // В этом случае даже если после чтения front_ второй поток вызовет Pop и изменит значение
        // front_ то наш метод просто завершится без добавления элемента, но гонки данных не
        // произойдёт
        if (back == front) {
            return false;  // Буфер полон
        }

        buffer_[back] = std::forward<U>(u);

        // Используем операцию остатка от деления, чтобы при достижении границы буфера обнулить
        // индекс back_
        back_.store((back + 1) % buffer_.size(), std::memory_order_release);
        return true;
    }

    std::optional<T> Pop() {
        // Ваш код здесь
        const size_t back = back_.load(std::memory_order_acquire);
        const size_t front = (front_.load(std::memory_order_relaxed) + 1) % buffer_.size();
        if (back == front) {
            return std::nullopt;  // Буфер пуст
        }

        std::optional<T> result(std::move(buffer_[front]));
        front_.store(front, std::memory_order_release);
        return result;
    }
};