#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <print>

std::string download_page(std::string const &url) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    return "content downloaded from " + url;
}

std::unordered_map<std::string, std::string> download_pages_with_mutex(std::vector<std::string> const &urls) {
    std::unordered_map<std::string, std::string> results;
    std::mutex results_mutex;
    results.reserve(urls.size());
    {
        std::vector<std::jthread> threads;
        threads.reserve(urls.size());

        for (size_t i = 0; i < urls.size(); ++i) {
            threads.emplace_back([&results, &results_mutex, &url = urls[i]]() {
                std::string content = download_page(url);

                // захватывает мьютекс, охраняющий results
                auto lk = std::lock_guard(results_mutex);
                results.emplace(url, std::move(content));
            });
        }
    }
    return results;
}

std::unordered_map<std::string, std::string> download_pages_without_mutex(std::vector<std::string> const &urls) {
    std::vector<std::string> contents(urls.size());

    {
        std::vector<std::jthread> threads;
        threads.reserve(urls.size());

        for (size_t i = 0; i < urls.size(); ++i) {
            threads.emplace_back([&contents, &urls, i]() { contents[i] = std::move(download_page(urls[i])); });
        }
    }

    std::unordered_map<std::string, std::string> results;
    results.reserve(urls.size());
    for (size_t i = 0; i < urls.size(); ++i) {
        results.emplace(urls[i], std::move(contents[i]));
    }

    return results;
}

int main() {
    std::vector<std::string> urls = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    std::println("Without mutex: {}", download_pages_without_mutex(urls));
    std::println("With mutex: {}", download_pages_with_mutex(urls));
}