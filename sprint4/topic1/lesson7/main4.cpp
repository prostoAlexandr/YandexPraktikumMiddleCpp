#include <print>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>

struct Shift {
    int x;
    int y;
};

struct Point {
    int x;
    int y;
};

auto get_shifted_grid(const int &start, const int &stop, const int &count, const Shift &shift) {
    // здесь ваш код
    using namespace ranges;
    auto x_points = views::linear_distribute(start, stop, count);
    auto y_points = x_points;
    return views::cartesian_product(x_points, y_points) | views::transform([shift](auto &&tuple) {
               auto [x, y] = tuple;
               return Point{x + shift.x, y + shift.y};
           });
}

int main() {
    auto result = get_shifted_grid(0, 1, 2, {1, 1});
    for (const auto &p : result) {
        std::println("x: {}, y: {}", p.x, p.y);
    }
}