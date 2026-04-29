#include <benchmark/benchmark.h>
#include <vector>
#include <map>
#include <flat_map>
#include <unordered_map>
#include <set>
#include <flat_set>
#include <unordered_set>
#include <random>
#include <algorithm>

using benchmark::DoNotOptimize;

std::unordered_map<size_t, std::vector<int>> cachedData;

const std::span<int> generateData(size_t N) {

    auto it = cachedData.find(N);
    if (it != cachedData.end()) {
        return it->second;
    }

    std::vector<int> data;
    data.reserve(N);

    for (int i = 0; i < N; ++i) {
        data.emplace_back(i);
    }

    // Перемешиваем вектор, чтобы порядок вставок был случайным
    std::shuffle(data.begin(), data.end(), std::mt19937{42});

    auto newIt = cachedData.try_emplace(it, N, std::move(data));
    return newIt->second;
}

template <typename Cont>
concept MapLike = requires(Cont cont) {
    cont.try_emplace(int{}, int{});
};


// Вставка в контейнер
template <typename Cont>
static void BM_AssociativeContLike_Insert(benchmark::State& state) {
  int count = state.range(0);
  auto data = generateData(count);

  for (auto _ : state)
  {
    {
        Cont cont;
        for (auto v : data) {
            if constexpr (MapLike<Cont>) DoNotOptimize(cont.try_emplace(v, v));
            else                         DoNotOptimize(cont.emplace(v));
        }

        state.PauseTiming();
    }
    state.ResumeTiming();
  }
}

// Поиск элементов в контейнере
template <typename Cont>
static void BM_AssociativeContLike_Lookup(benchmark::State& state) {
    int count = state.range(0);
    auto data = generateData(count);
  
    Cont cont;
    for (auto v : data) {
        if constexpr (MapLike<Cont>) DoNotOptimize(cont.try_emplace(v, v));
        else                         DoNotOptimize(cont.emplace(v));
    }

    std::vector<int> lookupKeys;
    lookupKeys.reserve(count);
    for (int i = count; i > 0; --i) {
        lookupKeys.emplace_back(i);
    }

    for (auto _ : state) {
        for (auto k : lookupKeys) {
            DoNotOptimize(cont.contains(k));
        }
    }
}

// Итерация по контейнеру
template <typename Cont>
static void BM_AssociativeContLike_Iterate(benchmark::State& state) {
    int count = state.range(0);
    auto data = generateData(count);
  
    Cont cont;
    for (auto v : data) {
        if constexpr (MapLike<Cont>) DoNotOptimize(cont.try_emplace(v, v));
        else DoNotOptimize(cont.emplace(v));
    }

    for (auto _ : state) {
        unsigned int counter = 0;
        for (const auto& p : cont) {
            for (auto it = cont.begin(); it != cont.end(); ++it) {
                DoNotOptimize(counter++);
            }
        }
    }
}


BENCHMARK(BM_AssociativeContLike_Insert<std::map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Insert<std::flat_map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Insert<std::unordered_map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_AssociativeContLike_Insert<std::set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Insert<std::flat_set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Insert<std::unordered_set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);



BENCHMARK(BM_AssociativeContLike_Lookup<std::map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Lookup<std::flat_map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Lookup<std::unordered_map<int, int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_AssociativeContLike_Lookup<std::set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Lookup<std::flat_set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Lookup<std::unordered_set<int>>)->Range(10, 100'000)->Iterations(10)->Unit(benchmark::kMicrosecond);


BENCHMARK(BM_AssociativeContLike_Iterate<std::map<int, int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Iterate<std::flat_map<int, int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Iterate<std::unordered_map<int, int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_AssociativeContLike_Iterate<std::set<int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Iterate<std::flat_set<int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AssociativeContLike_Iterate<std::unordered_set<int>>)->Range(10, 10'000)->Iterations(10)->Unit(benchmark::kMicrosecond);


// Точка входа для Google Benchmark
BENCHMARK_MAIN();