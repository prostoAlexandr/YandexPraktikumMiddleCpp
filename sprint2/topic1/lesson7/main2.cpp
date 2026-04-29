#include <cstddef>
#include <functional>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>

class Cache {
public:
    template <typename Derived, typename ParT>
    decltype(auto) compute(this Derived &&derived, ParT par) {
        static std::unordered_map<size_t, decltype(derived.Calculate(par))> _cache;
        auto h = std::hash<ParT>{}(par);
        if (!_cache.contains(h)) {
            _cache[h] = derived.Calculate(par);
        }
        return _cache.at(h);
    }
};

class Fibonacci : public Cache {
public:
    size_t Calculate(size_t n) {
        std::println("Calculating fibonacci {}", n);
        if (n <= 1)
            return n;
        // We have access to compute so use it to speedup
        return compute(n - 1) + compute(n - 2);
    }
};

class CreditScore : public Cache {
public:
    double Calculate(const std::string &person_id) { return CalculateCreditScore(person_id); }
    double CalculateCreditScore(const std::string &person_id) {
        std::println("Calculating credit score");
        return 3.14;
    }
};

int main() {
    Fibonacci fib;
    // в первый раз вычисляем
    auto result1 = fib.compute(30);
    // во второй раз достаём значение из кеша
    auto result2 = fib.compute(30);

    CreditScore cs;
    // в первый раз вычисляем
    auto result3 = cs.compute("123");
    // во второй раз достаём значение из кеша
    auto result4 = cs.compute("123");
}