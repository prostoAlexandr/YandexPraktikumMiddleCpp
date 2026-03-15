#include <iostream>
#include <memory>
#include <utility>
#include <print>

struct HeavyData {
    std::array<int, 1024> data;
};

void UseUniquePtr(std::unique_ptr<HeavyData> ptr) { std::print("unique_ptr holds pointer {}\n", static_cast<void*>(ptr.get())); }

void GetUseCount(std::shared_ptr<HeavyData> ptr) { std::print("shared_ptr used by {} objects\n", ptr.use_count()); }

int main() {
    // 1.
    std::unique_ptr<HeavyData> ptr1 = std::make_unique<HeavyData>();
    UseUniquePtr(std::move(ptr1));

    // 2.
    std::unique_ptr<HeavyData> ptr2 = std::make_unique<HeavyData>();
    std::unique_ptr<HeavyData> ptr3 = std::move(ptr2);

    // 3.
    std::shared_ptr<HeavyData> ptr4 = std::make_unique<HeavyData>();

    // 4.
    std::unique_ptr<HeavyData> ptr5 = std::make_unique<HeavyData>();
    GetUseCount(std::move(ptr5));

    return 0;
}