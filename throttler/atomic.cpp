#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <print>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

class Throttler {
    const int m_maxTokens;
    const int m_tokensPerSecond;
    const int m_sleepTime;
    int m_numTokens;
    std::jthread m_tokenizer;
    std::queue<std::reference_wrapper<std::atomic_int>> m_queue;
    std::mutex m_mut;

public:
    Throttler(int max_tokens, int tokens_per_second)
        : m_maxTokens(max_tokens), m_tokensPerSecond(tokens_per_second), m_numTokens(0),
          m_sleepTime(1000 / tokens_per_second) {
        m_tokenizer = std::jthread([this](std::stop_token st) {
            while (!st.stop_requested()) {
                std::unique_lock lk(m_mut);
                m_numTokens = std::min(m_numTokens + 1, m_maxTokens);
                while (!m_queue.empty() && m_numTokens > 0) {
                    std::atomic_int &token = m_queue.front();
                    m_queue.pop();
                    token.store(1);
                    token.notify_one();
                    --m_numTokens;
                }
                lk.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
            }

            std::unique_lock lk(m_mut);
            while (!m_queue.empty()) {
                std::atomic_int &token = m_queue.front();
                m_queue.pop();
                token.store(-1);
                token.notify_one();
            }
        });
    }

    int Throttle() {
        static constexpr int initial_value = 0;
        std::unique_lock lk(m_mut);
        // БЫСТРЫЙ ПУТЬ: если есть свободные токены и никто не ждет в очереди
        if (m_queue.empty() && m_numTokens > 0) {
            --m_numTokens;
            return 1;  // Уходим сразу, сэкономили кучу тактов и context switch
        }

        std::atomic_int token{initial_value};
        m_queue.push(std::ref(token));
        lk.unlock();

        token.wait(initial_value, std::memory_order::acquire);
        return token;
    }
};

void test_thread(int index, std::shared_ptr<Throttler> th) {
    int result = th->Throttle();
    std::println("{} {}", index, result);
}

int main() {
    std::vector<std::jthread> thv;
    auto th = std::make_shared<Throttler>(2, 1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for (int i = 0; i < 10; ++i) {
        thv.emplace_back(test_thread, i, th);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}