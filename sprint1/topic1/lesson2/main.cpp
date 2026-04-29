#include <cstddef>
#include <cstdlib>
#include <new>
#include <print>

struct HeavyObject {
    int32_t data[1000];
};

// ===== Ваш код здесь =====

//
// Функции, не вызывающие утечки памяти
//

size_t numAllocs = 0;
size_t numDeallocs = 0;
size_t numArrayAllocs = 0;
size_t numArrayDeallocs = 0;

void *operator new(std::size_t sz) {
    void *ptr = malloc(sz);

    if (!ptr || sz == 0) {
        throw std::bad_alloc();
    }

    ++numAllocs;
    return ptr;
}

void operator delete(void *ptr) noexcept {
    ++numDeallocs;
    free(ptr);
}
void operator delete(void *ptr, std::size_t sz) noexcept {
    ++numDeallocs;
    free(ptr);
}

void *operator new[](std::size_t sz) {
    void *ptr = malloc(sz);

    if (!ptr || sz == 0) {
        throw std::bad_alloc();
    }

    ++numArrayAllocs;
    return ptr;
}

void operator delete[](void *ptr) noexcept
{
    ++numArrayDeallocs;
    free(ptr);
}
void operator delete[](void *ptr, std::size_t sz) noexcept
{
    ++numArrayDeallocs;
    free(ptr);
}

// =========================

//
// Функции утечки памяти
//
void LeakArrayMemory() { new HeavyObject[10]; }
void LeakSomeMemory() { new HeavyObject{}; }

void ProperArrayManagement() {
    HeavyObject *ho_arr = new HeavyObject[5];
    delete[] ho_arr;
}

void ProperMemoryManagement() {
    HeavyObject *ho = new HeavyObject{};
    delete ho;
}

//
// Функции для теста 2
//
void CatchMeIfYouCanRecursively(unsigned int depth) {
    if (depth <= 0)
        return;

    HeavyObject *obj = new HeavyObject{};

    HeavyObject *temp = new HeavyObject{};

    if (depth % 2 == 0) {
        ProperMemoryManagement();
    }

    if (depth % 3 == 0) {
        LeakSomeMemory();
    }

    if (depth % 7 == 0) {
        ProperArrayManagement();
    }

    if (depth % 11 == 0) {
        LeakArrayMemory();
    }

    CatchMeIfYouCanRecursively(depth - 1);
    delete obj;
}

//
// Функции для теста 1
//
void MemoryLeaksWithFor(HeavyObject *templateObject) {
    HeavyObject *array1 = new HeavyObject[10];

    for (std::size_t sz = 0; sz < 10; ++sz) {
        HeavyObject *array2 = new HeavyObject[10];
        LeakSomeMemory();
        ProperMemoryManagement();
        delete[] array2;
    }

    delete templateObject;
    delete[] array1;
}

void CatchMeIfYouCan() {
    HeavyObject *obj = new HeavyObject{};

    ProperMemoryManagement();

    MemoryLeaksWithFor(obj);
}

int main(int argc, char **argv) {
    // Простая утечка памяти
    for (std::size_t sz = 0; sz < 100; ++sz) {
        LeakSomeMemory();
    }

    // Несколько вложенных функций с циклом
    CatchMeIfYouCan();

    // Несколько вложенных функций с рекурсией
    CatchMeIfYouCanRecursively(42);

    std::print("Number of allocations:   {}\n"
               "Number of deallocations: {}\n"
               "Number of array allocations:   {}\n"
               "Number of array deallocations: {}\n\n"
               "Number of leaks in total: {}\n\n",
               numAllocs, numDeallocs, numArrayAllocs, numArrayDeallocs,
               numAllocs - numDeallocs + numArrayAllocs - numArrayDeallocs);

    std::print("Jemalloc approx. number of leaks:   {}\n", 171);
}