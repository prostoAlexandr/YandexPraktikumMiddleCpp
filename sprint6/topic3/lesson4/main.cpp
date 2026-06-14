#include <mutex>
#include <optional>
#include <print>
#include <queue>
#include <thread>

template <typename DataType>
class SyncedQueue {
public:
    void Push(DataType d) {
        std::unique_lock lk(mutex_);
        q_.push(std::move(d));
    }
    std::optional<DataType> Pop() {
        std::unique_lock lk(mutex_);
        if (q_.empty()) {
            return std::nullopt;
        }
        auto tmp = std::move(q_.front());
        q_.pop();
        return tmp;
    }

    void StopAcceptPushes()
    {
        std::unique_lock lk(mutex_);
        active_ = false;
    }
    bool CanConsume()
    {
        std::unique_lock lk(mutex_);
        return !q_.empty() || active_;
    }

private:
    std::queue<DataType> q_;
    std::mutex mutex_;  // для синхронизации внутри очереди мы будем использовать мьютекс
    bool active_ = true;
};

using namespace std::chrono_literals;

int main() {
    SyncedQueue<int> q;

    //
    // Создаём три потока, добавляющие данные в очередь
    //
    std::jthread t1{[&q] {
        for (auto _ : std::views::iota(0, 1'000)) {
            std::this_thread::sleep_for(1ms);
            q.Push(1);
        }
    }};
    std::jthread t2{[&q] {
        for (auto _ : std::views::iota(0, 1'000)) {
            std::this_thread::sleep_for(1ms);
            q.Push(2);
        }
    }};
    std::jthread t3{[&q] {
        for (auto _ : std::views::iota(0, 1'000)) {
            std::this_thread::sleep_for(1ms);
            q.Push(3);
        }
    }};

    //
    // Создаём 1 поток, читающий данные из очереди
    //
    std::jthread t4{[&q] {
        for (auto _ : std::views::iota(0, 100)) {
            if (auto res = q.Pop(); res) {
                std::println("Value: {}", res.value());
            } else {
                std::println("Queue is empty!");
            }
            std::this_thread::sleep_for(1ms);
        }
    }};

    return 0;
}