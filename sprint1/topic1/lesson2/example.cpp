#include <jemalloc/jemalloc.h>
#include <new>
#include <print>

static std::size_t num_allocs{0};
static std::size_t num_deallocs{0};

void *operator new(std::size_t sz) {
    void *ptr = malloc(sz);

    if (!ptr || sz == 0) {
        throw std::bad_alloc();  // required by [new.delete.single]/3
    }

    ++num_allocs;
    return ptr;
}

void operator delete(void *ptr) noexcept {
    ++num_deallocs;
    free(ptr);
}
void operator delete(void *ptr, std::size_t sz) noexcept {
    ++num_deallocs;
    free(ptr);
}

struct HeavyObject {
    int data[1000];
};

void leak_memory_function() { new HeavyObject{}; }

int main(int argc, char **argv) {
    for (std::size_t sz = 0; sz < 1000; ++sz) {
        leak_memory_function();
    }

    // Dump allocator statistics to stderr.
    malloc_stats_print(NULL, NULL, NULL);

    std::print("Number of allocations:   {}\n"
               "Number of deallocations: {}\n",
               num_allocs, num_deallocs);
} 