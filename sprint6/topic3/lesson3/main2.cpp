#include <condition_variable>
#include <cstddef>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

using Task = std::function<void()>;

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency())  // здесь ваш код
    {
        workers.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(&ThreadPool::Worker, this);
        }
    }

    ~ThreadPool() {
        // здесь ваш код
        stopped = true;
        cv.notify_all();
        for (auto &w : workers) {
            if (w.joinable()) {
                w.join();
            }
        }
    }

    void Push(Task task) {
        // здесь ваш код
        {
            std::unique_lock lk(mut);
            queue.push(std::move(task));
        }
        cv.notify_one();
    }

private:
    void Worker() {
        // здесь ваш код
        while (!stopped) {
            std::unique_lock lk(mut);
            cv.wait(lk, [this] { return !queue.empty() || stopped; });
            if (stopped) {
                return;
            }

            Task task = std::move(queue.front());
            queue.pop();
            lk.unlock();
            task();
        }
    }

    // здесь ваш код
    std::vector<std::jthread> workers;
    std::queue<Task> queue;
    std::mutex mut;
    std::condition_variable cv;
    std::atomic<bool> stopped{false};
};

int main() {}