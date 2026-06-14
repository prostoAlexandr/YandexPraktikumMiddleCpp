#include <algorithm>
#include <chrono>
#include <execution>
#include <print>
#include <random>
#include <ranges>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rv = std::ranges::views;
namespace rs = std::ranges;

const std::vector<std::string> tech_keywords = {"machine-learning",
                                                "quantum-computing",
                                                "transformer",
                                                "diffusion",
                                                "trading",
                                                "business",
                                                "video",
                                                "image",
                                                "games"};

std::vector<std::string> GenerateWords(size_t word_count, size_t keywords_insert_count) {
    std::vector<std::string> dictionary = {"the",     "a",         "in",         "system", "approach",
                                           "network", "signal",    "processing", "model",  "data",
                                           "node",    "structure", "input",      "output", "parameter"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dict_dist(0, dictionary.size() - 1);
    std::uniform_int_distribution<> keyword_dist(0, tech_keywords.size() - 1);

    std::vector<std::string> words;
    words.reserve(word_count + keywords_insert_count);

    for (size_t i : rv::iota(0u, word_count)) {
        words.push_back(dictionary[dict_dist(gen)]);
    }
    for (size_t i : rv::iota(0u, keywords_insert_count)) {
        words.push_back(tech_keywords[keyword_dist(gen)]);
    }
    rs::shuffle(words, gen);

    return words;
}

int main() {
    auto text = GenerateWords(50'000'000, 5'000'000);

    std::unordered_set<std::string_view> tech_keywords_set{tech_keywords.begin(), tech_keywords.end()};

    {
        auto start = std::chrono::high_resolution_clock::now();
        auto occurrences = std::count_if(std::execution::seq, text.begin(), text.end(),
                                         [&](const auto &s) { return tech_keywords_set.contains(s); });
        auto end = std::chrono::high_resolution_clock::now();

        std::println("\n'seq' policy - analyzed for {} milliseconds\n",
                     std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

        std::println("Tech keywords occurrences: {}", occurrences);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        auto occurrences = std::count_if(std::execution::par_unseq, text.begin(), text.end(),
                                         [&](const auto &s) { return tech_keywords_set.contains(s); });
        auto end = std::chrono::high_resolution_clock::now();

        std::println("\n'par_unseq' policy - analyzed for {} milliseconds\n",
                     std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

        std::println("Tech keywords occurrences: {}", occurrences);
    }

    return 0;
}