#include <cstdint>
#include <functional>
#include <thread>

struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct Image {
    std::vector<Pixel> pixels;
    int width, height;
};

struct GrayScaleFilter {
    void Filter(Image &image);  // thread-safe
};

struct ThreadPool {
    ThreadPool(size_t num_threads);
    void push(std::function<void()> task);
    ~ThreadPool();
};

void SaveImage(const Image &image);  // thread-safe

struct ImageProcessor {
    ImageProcessor();  // ваш код здесь
    void Process(Image &&image) {
        // ваш код здесь
        tp.push([image = std::move(image)] mutable {
            GrayScaleFilter{}.Filter(image);
            SaveImage(image);
        });
    }

private:
    // ваш код здесь
    ThreadPool tp = std::thread::hardware_concurrency();
};

int main() {}