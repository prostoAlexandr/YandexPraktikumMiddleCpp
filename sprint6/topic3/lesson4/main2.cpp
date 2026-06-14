#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <thread>

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

template <typename DataType>
class SyncedQueue {
public:
    SyncedQueue(size_t capacity) : capacity_(capacity) {}

    void Push(DataType d) {
        std::unique_lock lk(mutex_);
        not_full_.wait(lk, [this] { return q_.size() < capacity_ || !active_; });
        if (!active_) {
            return;
        }
        q_.push(std::move(d));
        lk.unlock();
        not_empty_.notify_one();
    }

    std::optional<DataType> Pop() {
        std::unique_lock lk(mutex_);
        not_empty_.wait(lk, [this] { return !q_.empty() || !active_; });
        if (q_.empty()) {
            return std::nullopt;
        }
        auto result = std::move(q_.front());
        q_.pop();
        lk.unlock();
        not_full_.notify_one();
        return result;
    }

    void StopAcceptPushes() {
        std::unique_lock lk(mutex_);
        active_ = false;
        not_full_.notify_all();
        not_empty_.notify_all();
    }

private:
    std::queue<DataType, std::list<DataType>> q_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;

    size_t capacity_;
    bool active_ = true;
};

template <typename T>
class Producer {
public:
    Producer(SyncedQueue<T> &queue, size_t total) : queue_(queue), remaining_(total) {
        thread_ = std::jthread([this] { Run(); });
    }

    void Wait() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    size_t Size() const { return remaining_; }

private:
    void Run() {
        for (; remaining_ > 0; --remaining_) {
            queue_.Push(remaining_);
        }
    }

    SyncedQueue<T> &queue_;
    std::atomic<size_t> remaining_{0};
    std::jthread thread_;
};

template <typename T>
class CountedConsumer {
public:
    explicit CountedConsumer(SyncedQueue<T> &queue) : queue_(queue) {
        thread_ = std::jthread([this] { Run(); });
    }

    void Wait() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    size_t Size() const { return count_; }

private:
    void Run() {
        while (true) {
            auto data = queue_.Pop();
            if (data.has_value()) {
                ++count_;
            }
            else{
                return;
            }
        }
    }

    SyncedQueue<T> &queue_;
    std::atomic<size_t> count_{0};
    std::jthread thread_;
};

namespace rv = std::views;
namespace rs = std::ranges;

int main() {
    std::println("Запускаем тест SyncedQueue");

    SyncedQueue<size_t> queue(100);

    size_t num_producers = 5;
    size_t num_consumers = 5;
    size_t num_tasks_for_one_producer = 100'000;

    std::list<Producer<size_t>> txs;
    for (auto _ : rv::iota(0u, num_producers)) {
        txs.emplace_back(queue, num_tasks_for_one_producer);
    }

    std::list<CountedConsumer<size_t>> rxs;
    for (auto _ : rv::iota(0u, num_consumers)) {
        rxs.emplace_back(queue);
    }

    // Ждём, пока производители добавят все задачи в очередь, и останавливаем обработку новых задач
    rs::for_each(txs, [](auto &tx) { tx.Wait(); });

    queue.StopAcceptPushes();

    // Ждём, пока потребители обработают все задачи
    std::println("Wait for consumers");
    rs::for_each(rxs, [](auto &rx) {
        rx.Wait();
        rx.Wait();
    });

    // Проверяем, что Producer добавил все свои задачи в очередь
    if (rs::any_of(txs, [](auto &tx) { return tx.Size() != 0; })) {
        std::println("Ошибка! Производители отправили не все задачи на исполнение");
    }

    // Проверяем, что CountedConsumer обработал все задачи
    size_t totally_consumed_tasks =
        std::accumulate(rxs.begin(), rxs.end(), 0u,
                        [](size_t sum, CountedConsumer<size_t> &rx) { return sum + rx.Size(); });

    if (totally_consumed_tasks != (num_tasks_for_one_producer * num_producers)) {
        std::println("Ошибка! Потребители обработали не все задачи");
    }

    std::println("Success!");
    return 0;
}