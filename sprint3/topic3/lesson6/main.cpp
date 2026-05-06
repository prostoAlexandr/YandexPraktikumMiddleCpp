#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <random>
#include <vector>

void AnalyzeSteps() {
    std::vector<int> steps(30);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(5000, 15000);
    std::generate(steps.begin(), steps.end(), std::bind(dist, rng));

    int totalSt = 0;
    int avrSt = 0;
    int maxSt = 0;
    int dayWithMaxSt = 0;

    // Ваш код здесь
    totalSt = std::reduce(steps.begin(), steps.end());
    avrSt = totalSt / steps.size();
    auto maxit = std::max_element(steps.begin(), steps.end());
    maxSt = *maxit;
    dayWithMaxSt = std::distance(steps.begin(), maxit);

    std::print("\nВаш прогресс:\n");
    for (auto target : {avrSt + 3000, avrSt + 500, avrSt - 3000}) {
        std::string line(steps.size(), ' ');

        // Ваш код здесь
        auto it = std::find_if(steps.begin(), steps.end(), [target](int val) { return val > target; });

        std::print("{}\n", line);
    }
    std::print("Дни: от 1 до 30\n\n");

    int meanSt = 0;

    std::print("Ваша статистика за последние 30 дней:"
               "\n\tВсего шагов: {}"
               "\n\tШагов в среднем за день: {}"
               "\n\tШагов в середине месяца: {}"
               "\n\tСамый продуктивный день - {}. Вы совершили {} шагов.\n\n",
               totalSt, avrSt, meanSt, maxSt, dayWithMaxSt);

    std::print("Ваш прогресс:\n");

    for (auto target : {avrSt + 3000, avrSt + 500, avrSt - 3000}) {
        std::string line(steps.size(), ' ');

        // Ваш код здесь

        std::print("{}\n", line);
    }
    std::print("Дни: от 1 до 30\n");
}

int main() { AnalyzeSteps(); }