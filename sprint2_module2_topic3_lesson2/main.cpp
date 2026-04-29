#include <algorithm>
#include <print>

template <typename CharT, std::size_t N>
struct fixed_string {
    CharT data[N]{};
    /* Реализуйте здесь конструктор, принимающий массив символов */
    constexpr fixed_string(const CharT (&str)[N]) { std::copy_n(str, N, data); }
};

template <fixed_string Str>
struct FormatString {
    void test_print() const { std::println(Str.data, "Hello, ", "world!"); }
};

int main() {
    constexpr FormatString<"{} {}"> fs;
    fs.test_print();
}