#include <cstdlib>
#include <memory>

class Image {
public:
    Image(unsigned int width, unsigned int height) : width_(width), height_(height) {}

private:
    unsigned int width_;
    unsigned int height_;
};

int main() {
    const std::size_t len = 5;
    const unsigned int hl = 256;
    auto *arrPtrPN = (Image *)std::malloc(sizeof(Image) * len);
    // Ваш код здесь
    for (size_t i = 0; i < len; ++i)
    {
        new (std::addressof(arrPtrPN[i])) Image(hl, hl);
        arrPtrPN[i].~Image();
    }
    for (size_t i = 0; i < len; ++i)
    {
        new (std::addressof(arrPtrPN[i])) Image(hl, hl);
        arrPtrPN[i].~Image();
    }
    for (size_t i = 0; i < len; ++i)
    {
        auto ptr = std::construct_at<Image>(std::addressof(arrPtrPN[i]), hl, hl);
        std::destroy_at(ptr);
    }
    free(arrPtrPN); // Fix of memory leak
    return 0;
}