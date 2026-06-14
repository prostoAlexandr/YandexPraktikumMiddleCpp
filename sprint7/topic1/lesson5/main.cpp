#include <chrono>
#include <future>
#include <iostream>
#include <optional>
#include <thread>
#include <vector>

using namespace std::literals::chrono_literals;

std::int64_t fib(int64_t n) {
    if (n <= 1) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

class Worker {
public:
    explicit Worker(size_t poolSize) : futures_(poolSize) {}

    bool run(int n) {
        if (auto index = findSlot()) {
            std::packaged_task<void()> task([n] {
                auto r = fib(n);
                std::cout << "nth fib: " << r << std::endl;
            });
            futures_.at(*index) = task.get_future();
            std::thread thread(std::move(task));
            thread.detach();
            return true;
        }
        return false;
    }

private:
    std::optional<size_t> findSlot() {
        for (size_t i = 0; i < futures_.size(); ++i) {
            if (!futures_.at(i)) {
                return i;
            }
            if (futures_.at(i)->wait_for(0s) == std::future_status::ready) {
                futures_.at(i).reset();
                return i;
            }
        }
        return {};
    }

private:
    std::vector<std::optional<std::future<void>>> futures_;
};

int main() {
    Worker worker(10);
    int n = 0;
    while (std::cin >> n) {
        while (!worker.run(n)) {
            std::this_thread::sleep_for(500ms);
        }
    }
}