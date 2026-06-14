#include <atomic>
#include <chrono>
#include <iostream>
#include <print>
#include <thread>

std::atomic<bool> lock1 = false;
std::atomic<bool> lock2 = false;

// Поток 1: захватывает lock1, затем пытается захватить lock2
void thread1_work() {
    while (true) {
        // Захватываем lock1
        while (lock1.exchange(true))
            ;
        std::println("Thread 1 has acquired lock1, try to acquire lock2...");
        // Пытаемся захватить lock2
        if (!lock2.exchange(true)) {
            // Успешно захватили оба лока!
            std::println("Thread 1 has acquired both locks!");
            lock2 = false;
            lock1 = false;
            break;
        } else {
            // Не получилось, отпускаем lock1 и пробуем снова
            std::println("Thread 1 failed to acquire lock2, release lock1...");
            lock1 = false;
        }
    }
}

// Поток 2: захватывает lock2, затем пытается захватить lock1
void thread2_work() {
    while (true) {
        // Захватываем lock2
        while (lock2.exchange(true))
            ;
        std::println("Thread 2 has acquired lock2, try to acquire lock1...");
        // Пытаемся захватить lock1
        if (!lock1.exchange(true)) {
            // Успешно захватили оба лока!
            std::println("Thread 2 has acquired both locks!");
            lock1 = false;
            lock2 = false;
            break;
        } else {
            // Не получилось, отпускаем lock2 и пробуем снова
            std::println("Thread 2 failed to acquire lock1, release lock2...");
            lock2 = false;
        }
    }
}

int main() {
    std::jthread t1(thread1_work);
    std::jthread t2(thread2_work);
}