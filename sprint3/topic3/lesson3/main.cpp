#include <algorithm>
#include <functional>
#include <iterator>
#include <print>
#include <string>
#include <vector>

bool is_prime(int n) {
    if (n < 2)
        return false;
    for (int d = 2; d * d <= n; ++d)
        if (n % d == 0)
            return false;
    return true;
}

int main() {
    std::vector numbers = {4, 6, 4, 4, 6, 8, 10, 13, 14, 14, 14, 15};

    // 1
    // int first_prime = -1;
    // for (int x : numbers) {
    //     if (is_prime(x)) {
    //         first_prime = x;
    //         break;
    //     }
    // }
    auto find_it = std::find_if(numbers.begin(), numbers.end(), is_prime);
    int first_prime = find_it == numbers.end() ? -1 : *find_it;

    // 2
    // bool all_even = true;
    // for (int x : numbers) {
    //     if (x % 2 != 0) {
    //         all_even = false;
    //         break;
    //     }
    // }
    bool all_even = std::all_of(numbers.begin(), numbers.end(), [](int x){return x % 2 == 0;});

    // 3
    // int adjacent_duplicate = -1;
    // for (size_t i = 0; i + 1 < numbers.size(); ++i) {
    //     if (numbers[i] == numbers[i + 1]) {
    //         adjacent_duplicate = numbers[i];
    //         break;
    //     }
    // }
    auto adj_it = std::adjacent_find(numbers.begin(), numbers.end());
    int adjacent_duplicate = adj_it == numbers.end() ? -1 : *adj_it;

    // 4
    std::vector<int> other = {4, 6, 4, 4, 6, 8, 10, 13, 14, 14, 15, 15};
    // size_t mismatch_index = -1;
    // for (size_t i = 0; i < std::min(numbers.size(), other.size()); ++i) {
    //     if (numbers[i] != other[i]) {
    //         mismatch_index = i;
    //         break;
    //     }
    // }
    auto mismatch = std::mismatch(numbers.begin(), numbers.end(), other.begin(), other.end());
    size_t mismatch_index = mismatch.first == numbers.end() ? -1 : std::distance(numbers.begin(), mismatch.first);

    // 5
    std::string text = "This is a very long text and I want to use the most efficient algorithm here";
    std::string word = "algorithm";
    // size_t found_index = std::string::npos;
    // for (size_t i = 0; i + word.size() <= text.size(); ++i) {
    //     if (text.substr(i, word.size()) == word) {
    //         found_index = i;
    //         break;
    //     }
    // }
    auto it = std::search(text.begin(), text.end(), std::boyer_moore_searcher(word.begin(), word.end()));
    size_t found_index = (it == text.end()) ? std::string::npos : std::distance(text.begin(), it);

    // Вывод результатов
    std::println("First prime: {}", first_prime);
    std::println("All even: {}", all_even);
    std::println("Adjacent duplicate: {}", adjacent_duplicate);
    std::println("First mismatch index: {}", mismatch_index);
    std::println("Substring found at index: {}", found_index);
}