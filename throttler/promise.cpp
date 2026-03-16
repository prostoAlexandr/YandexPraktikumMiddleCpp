#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <iostream>
#include <vector>

class Throttler
{
    int m_maxTokens;
    int m_tokensPerSecond;
    std::queue<std::promise<bool>> m_queue;
    std::mutex m_mutQueue;
    std::jthread m_tokenizer;

public:
    Throttler(int max_tokens, int tokens_per_second) :
        m_maxTokens(max_tokens),
        m_tokensPerSecond(tokens_per_second)
    {
        m_tokenizer = std::jthread([this](std::stop_token stopped){
            int tokens = m_maxTokens;
            while (!stopped.stop_requested())
            {
                tokens = std::min(tokens + 1, m_maxTokens);
                std::unique_lock sl(m_mutQueue);
                while (tokens > 0 && !m_queue.empty())
                {
                    m_queue.front().set_value(true);
                    m_queue.pop();
                    --tokens;
                }
                sl.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_tokensPerSecond));
            }
        });
    }

    bool Throttle()
    {
        std::unique_lock sl(m_mutQueue);
        m_queue.emplace();
        std::future<bool> fut = m_queue.back().get_future();
        sl.unlock();
        return fut.get();
    }

    ~Throttler()
    {
        m_tokenizer.request_stop();
        std::unique_lock sl(m_mutQueue);
        while (!m_queue.empty())
        {
            m_queue.front().set_value(false);
            m_queue.pop();
        }
    }
};

void test_thread(int index, std::shared_ptr<Throttler> th)
{
    bool result = th->Throttle();
    std::cout << index << " " << result << std::endl;
}

int main()
{
    std::vector<std::jthread> thv;
    auto th = std::make_shared<Throttler>(1, 1);
    for (int i = 0; i < 10; ++i)
    {
        thv.emplace_back(test_thread, i, th);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}