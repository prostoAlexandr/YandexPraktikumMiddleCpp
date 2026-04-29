#include <span>
#include <algorithm>
#include <vector>
#include <cassert>

constexpr void process(std::span<int> s) { 
  std::for_each(s.begin(), s.end(),
    [](auto& x) {
      x *= x;
  });
};

int main() {
  std::vector v = {1, 2, 3};
  process(v);
  assert((v == std::vector{1, 4, 9}));
}