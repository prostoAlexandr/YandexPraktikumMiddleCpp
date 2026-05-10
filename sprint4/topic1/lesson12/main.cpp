#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>
#include <print>

struct ParticleEvent {
    unsigned event_id;
    double total_energy;  // ГэВ
    // и ещё много полей
};

std::vector<size_t> buildIndex(std::vector<ParticleEvent> const &events) {
    std::vector<size_t> indices(events.size());
    std::ranges::iota(indices, 0);  // заполняет вектор последовательными значениями, начиная с 0
    std::ranges::sort(indices, std::greater{}, [&events](const auto ind){return events[ind].total_energy;});
    return indices;
}

int main()
{
    std::vector<ParticleEvent> events = {{1, 1}, {2, 10}, {3, 50}, {4, 15}, {5, 0}};
    std::println("{}", buildIndex(events));
}