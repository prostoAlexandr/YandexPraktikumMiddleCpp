#include <atomic>
#include <iostream>
#include <string>
#include <vector>

std::atomic<bool> flushing(false);
std::vector<std::string> log_buffer;

void logger_thread() {
    while (true) {
        while (!flushing.load(std::memory_order_relaxed))
            ;
        std::atomic_thread_fence(std::memory_order_acquire);
        std::cout << "Flushing begin" << std::endl;
        for (const auto &msg : log_buffer) {
            std::cout << msg << '\n';
        }
        log_buffer.clear();
        std::atomic_thread_fence(std::memory_order_release);
        flushing.store(false, std::memory_order_relaxed);
    }
}

void log_message(std::string_view msg) {
    while (flushing.load(std::memory_order_relaxed))
        ;
    std::atomic_thread_fence(std::memory_order_acquire);
    log_buffer.emplace_back(msg);
    if (log_buffer.capacity() == log_buffer.size()) {
        std::atomic_thread_fence(std::memory_order_release);
        flushing.store(true, std::memory_order_relaxed);
    }
}

/*
В этой реализации каждая итерация цикла выполнения while заставляет процессор выполнять чтение с
семантикой acquire.

На архитектуре x86-64 это почти бесплатно, так как любое чтение из памяти там по умолчанию имеет
семантику acquire.

На ARM (например, ARMv8) компилятор вынужден генерировать инструкцию LDAR (Load-Acquire) на каждую
итерацию. Это создает серьезную нагрузку на конвейер процессора, запрещая ему спекулятивно
переупорядочивать инструкции внутри цикла и оптимизировать предсказание переходов.
*/