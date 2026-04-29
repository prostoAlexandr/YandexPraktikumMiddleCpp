#include <memory>
class Image {
public:
    Image(unsigned int width, unsigned int height) : width_(width), height_(height) {}

private:
    unsigned int width_;
    unsigned int height_;
};

class VectorOfImages {
public:
    ~VectorOfImages() { std::destroy_n(GetValidStoragePtr(), endPos_); }
    Image *GetValidStoragePtr() { return reinterpret_cast<Image *>(std::addressof(storage_)); }
    void EmplaceBack(unsigned int width, unsigned int height) {
        if (endPos_ >= maxSize_) {
            throw std::bad_alloc{};
        }
        // Ваш код здесь
        std::construct_at(GetValidStoragePtr() + endPos_, width, height);
        ++endPos_;
    }

private:
    unsigned int endPos_ = 0;
    static const unsigned int maxSize_ = 20;
    alignas(alignof(Image)) std::byte storage_[maxSize_ * sizeof(Image)];  // Ваш код здесь
};

int main() {
    VectorOfImages voi;
    voi.EmplaceBack(256, 256);
}