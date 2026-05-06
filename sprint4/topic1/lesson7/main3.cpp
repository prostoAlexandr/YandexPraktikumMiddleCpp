#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

using namespace ranges;

auto get_viral_posts(const std::vector<int> &likes, const std::vector<int> &shares) {
    return views::zip(likes, shares) | views::enumerate | views::adjacent_filter([](auto &&lhs, auto &&rhs) {
               const auto &[lindex, lranges] = lhs;
               const auto &[rindex, rranges] = rhs;
               const auto &[llnum, lsnum] = lranges;
               const auto &[rlnum, rsnum] = rranges;
               return rlnum + rsnum >= (llnum + lsnum) * 3;
           }) | views::transform([](auto && tuple){return std::get<0>(tuple);});
}

int main() {
    std::vector<int> likes = {3, 5, 15, 2, 9, 1, 3, 17, 18};
    std::vector<int> reposts = {2, 4, 22, 1, 17, 1, 3, 1, 7};
    auto result = get_viral_posts(likes, reposts);
}