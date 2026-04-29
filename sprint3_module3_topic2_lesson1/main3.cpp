#include <vector>
#include <mdspan>
#include <print>

void printmd(auto mds) { 
    for (size_t i = 0; i < mds.extent(0); ++i)
    {
        for (size_t j = 0; j < mds.extent(1); ++j)
        {
            std::print("{}", mds[i, j]);
        }
        std::println();
    }
};

int main() {
  std::vector v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto mds = std::mdspan(v.data(), 3, 3);
  // auto mds = std::mdspan<int, std::extents<int, 3, 3>>(v);
  printmd(mds);
}
