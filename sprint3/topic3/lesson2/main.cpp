#include <algorithm>
#include <map>
#include <print>
#include <set>
#include <string>
#include <vector>

class DataProcessor {
public:
    DataProcessor() {
        numbers_ = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        uniqueWords_ = {"apple", "banana", "cherry", "date", "elderberry"};
        wordFrequency_ = {{"apple", 3}, {"banana", 1}, {"cherry", 2}, {"date", 5}, {"elderberry", 0}};
    }

    void removeEvenNumbers() {
        // numbers_.erase(std::remove_if(numbers_.begin(), numbers_.end(), [](int x) { return x % 2 == 0; }),
        //                numbers_.end());
        std::erase_if(numbers_, [](int x) { return x % 2 == 0; });
    }

    void removeSpecificNumber(int val) {
        // auto it = std::remove(numbers_.begin(), numbers_.end(), val);
        // numbers_.erase(it, numbers_.end());
        std::erase(numbers_, val);
    }

    void removeShortWords(int minLen) {
        // uniqueWords_.erase(std::remove_if(uniqueWords_.begin(), uniqueWords_.end(),
        //                                   [minLen](const std::string &s) { return s.length() < minLen; }),
        //                    uniqueWords_.end());
        std::erase_if(uniqueWords_, [minLen](const std::string &s) { return s.length() < minLen; });
    }

    void removeLongWords(int maxLen) {
        // auto it = std::remove_if(uniqueWords_.begin(), uniqueWords_.end(),
        //                          [maxLen](const std::string &s) { return s.length() > maxLen; });
        // uniqueWords_.erase(it, uniqueWords_.end());
        std::erase_if(uniqueWords_, [maxLen](const std::string &s) { return s.length() > maxLen; });
    }

    void removeWordsWithoutOccurrence() {
        // wordFrequency_.erase(std::remove_if(wordFrequency_.begin(), wordFrequency_.end(),
        //                                     [](const auto &pair) { return pair.second == 0; }),
        //                      wordFrequency_.end());
        std::erase_if(wordFrequency_, [](const auto &pair) { return pair.second == 0; });
    }

    void printResults() {
        // std::print("\nNumbers after processing: {}"
        //            "\nUnique words: {}"
        //            "\nWord frequencies: {}\n",
        //            numbers_, uniqueWords_, wordFrequency_);
    }

private:
    std::vector<int> numbers_;
    std::set<std::string> uniqueWords_;
    std::map<std::string, int> wordFrequency_;
};

int main() {
    DataProcessor processor;
    processor.removeEvenNumbers();
    processor.removeSpecificNumber(5);
    processor.removeShortWords(5);
    processor.removeLongWords(6);
    processor.removeWordsWithoutOccurrence();
    processor.printResults();

    return 0;
}