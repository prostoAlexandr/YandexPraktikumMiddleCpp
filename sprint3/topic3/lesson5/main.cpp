#include <algorithm>
#include <string>
#include <vector>

struct Book {
    std::string name;
    int idx = 0;
    auto operator<=>(const Book &rhd) const { return idx <=> rhd.idx; }
};

void Process(std::vector<Book>& books) {
    // if (books.empty())
    //     return;

    // bool flag = false;
    // for (auto [cur, next] = std::make_pair(books.begin(), std::next(books.begin())); next != books.end();
    //      ++cur, ++next) {
    //     if (*cur > *next) {
    //         flag = true;
    //         break;
    //     }
    // }

    // if (!flag) {
    //     int n = books.size();
    //     for (int i = 0; i < n - 1; ++i) {
    //         for (int j = 0; j < n - i - 1; ++j) {
    //             if (books[j] > books[j + 1]) {
    //                 std::swap(books[j], books[j + 1]);
    //             }
    //         }
    //     }

    //     auto medianVal = *std::next(books.begin(), books.size() / 2);
    //     for (auto it = books.begin(); it != books.end();) {
    //         if (*it < medianVal) {
    //             it = books.erase(it);
    //         } else {
    //             ++it;
    //         }
    //     }
    // } else {

    //     auto medianVal = *std::next(books.begin(), books.size() / 2);
    //     for (auto it = books.begin(); it != books.end();) {
    //         if (*it < medianVal) {
    //             it = books.erase(it);
    //         } else {
    //             ++it;
    //         }
    //     }
    // }

    auto median = std::next(books.begin(), books.size() / 2);
    std::nth_element(books.begin(), median, books.end());
    books.erase(books.begin(), median);
}

int main()
{
    std::vector<Book> books{{"AAA", 1}, {"BBB", 2}, {"CCC", 3}, {"DDD", 4}};
    Process(books);
}