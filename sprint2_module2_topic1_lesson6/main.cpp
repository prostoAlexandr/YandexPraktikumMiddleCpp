#include <print>
#include <type_traits>

struct pointer_tag {};
struct non_pointer_tag {};
template <typename T>
struct type_category {
    using type = non_pointer_tag;
};
template <typename T>
struct type_category<T *> {
    using type = pointer_tag;
};
template <typename T>
void process_impl(T, non_pointer_tag) {
    std::println("Non-pointer type");
}
template <typename T>
void process_impl(T, pointer_tag) {
    std::println("Pointer type");
}
template <typename T>
void process(T t) {
    // Ваш код здесь
    process_impl(t, typename type_category<T>::type());
}

int main() {
    int test = 42;
    int *testptr = &test;
    process(test);
    process(testptr);
}