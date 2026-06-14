#include <algorithm>
#include <benchmark/benchmark.h>
#include <execution>
#include <random>
#include <vector>

/*
sudo apt install libtbb-dev
g++ -o app --std=gnu++23 -O3 -march=native -I/home/dev/.conan2/p/b/bench708e5cd3bf306/b/src/include -L/home/dev/.conan2/p/b/bench708e5cd3bf30
6/b/build/Debug/src/ main.cpp -lbenchmark -lbenchmark_main -ltbb
*/

template <typename ExecPolicy>
void BM_CopyIf(benchmark::State &state, ExecPolicy policy) {
    const int size = state.range(0);

    // Создаём массив с данными и перемешиваем его
    std::vector<int> source(size);
    std::iota(source.begin(), source.end(), 1);
    std::shuffle(source.begin(), source.end(), std::mt19937{});

    std::vector<int> result;
    result.resize(size);

    for (auto _ : state) {
        // std::copy_if(policy, source.begin(), source.end(), result.begin(), [](int x) { return x % 2 == 0; });
        // std::transform(policy, source.begin(), source.end(), result.begin(), [](int x) { return x + 10; });
        // benchmark::DoNotOptimize(std::reduce(policy, source.begin(), source.end(), 0, std::plus<int>{}));
        std::transform(policy, source.begin(), source.end(), result.begin(), [](int x) { return std::sqrt(std::sin(x) + std::cos(5 * x)); });

        // Запрещаем компилятору удалять операции с result (ведь дальше в коде он не используется)
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_CAPTURE(BM_CopyIf, seq, std::execution::seq)
    ->Range(1000, 100'000'000)
    ->Iterations(20)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_CopyIf, unseq, std::execution::unseq)
    ->Range(1000, 100'000'000)
    ->Iterations(20)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_CopyIf, par, std::execution::par)
    ->Range(1000, 100'000'000)
    ->Iterations(20)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_CopyIf, par_unseq, std::execution::par_unseq)
    ->Range(1000, 100'000'000)
    ->Iterations(20)
    ->Unit(benchmark::kMicrosecond);