#include <cstdlib>
#include <memory>


int main()
{
    const std::size_t size = 10;
    auto *pArr = static_cast<int *>(std::malloc(size * sizeof(int)));
    auto *pValue = static_cast<double *>(std::malloc(sizeof(double)));
    std::free(pValue);
    std::free(pArr); 

    auto pArr_uptr = std::make_unique<int[]>(size);
    auto pValue_uptr = std::make_unique<double>();
}