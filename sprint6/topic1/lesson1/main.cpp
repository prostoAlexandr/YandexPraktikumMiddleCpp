#include <algorithm>
#include <functional>
#include <span>
#include <thread>
#include <vector>
#include <ranges>

// Функция, которая будет запускаться в отдельных потоках
template <typename T>
void find_chunk_min(std::span<const T> chunk, T &result_location) {
    result_location = std::ranges::min(chunk);
}

template <typename T>
T parallel_min(const std::vector<T> &data) {
    if (data.empty()) {
        throw std::invalid_argument("Cannot find minimum of empty vector");
    }

    // Сколько в процессоре ядер. Вернёт 0, если нет поддержки потоков
    const size_t max_threads = std::thread::hardware_concurrency();

    constexpr size_t MT_THRESHOLD = 1024 * 16;
    // Многопоточность выгодна, только если элементов больше, чем MT_THRESHOLD
    // Здесь MT_THRESHOLD искусственно занижен, чтобы код использовал
    // многоточность без необходимости заводить много мегабайт данных
    if (data.size() <= MT_THRESHOLD || max_threads <= 1) {
        return std::ranges::min(data);
    }

    // Гарантируем, что ни один поток не получит пустой блок данных
    const size_t num_threads = std::min(max_threads, data.size());

    std::vector<T> partial_mins(num_threads);  // результаты работы потоков
    std::vector<std::jthread> threads;
    threads.reserve(data.size());

    //
    // Ваш код здесь
    //
    for (auto i : std::views::iota(0, data.size()))
    {
        threads.emplace_back(find_chunk_min, std::ref(data[i]), std::ref(partial_mins[i]));
    }

    // Минимум среди частичных минимумов
    return std::ranges::min(partial_mins);
}

int main() {}