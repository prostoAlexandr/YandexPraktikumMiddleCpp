#include <atomic>
#include <print>
#include <thread>

int data = 0;             // Исходные данные
int processed_data1 = 0;  // Результат Processor1 (не атомарный!)
int processed_data2 = 0;  // Результат Processor1 (не атомарный!)
// ваш код здесь
std::atomic<bool> produced{false};
std::atomic<bool> processed1{false};
std::atomic<bool> processed2{false};

void producer() {
    // ваш код здесь
    data = 42;
    produced.store(true, std::memory_order::release);
}

void processor1() {
    // ваш код здесь
    while (!produced.load(std::memory_order::acquire));
    processed_data1 = data + 2;
    processed1.store(true, std::memory_order::release);
}

void processor2() {
    // ваш код здесь
    while (!produced.load(std::memory_order::acquire));
    processed_data2 = data + 3;
    processed2.store(true, std::memory_order::release);
}

void printer() {
    // ваш код здесь
    while (!processed1.load(std::memory_order::acquire) || !processed2.load(std::memory_order::acquire));
    std::print("{} {}", processed_data1, processed_data2);
}

int main() {
    std::thread t4(printer), t3(processor2), t2(processor1), t1(producer);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}