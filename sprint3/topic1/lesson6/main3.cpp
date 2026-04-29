#include <iostream>
#include <string>
#include <cstring>

extern "C" void c_log(const char *c_str) { printf("%lld | %5ld: %s\n", (long long)time(NULL), strlen(c_str), c_str); }

int main() {
    std::string result;
    while (std::cin) {
        std::cin >> result;
        c_log(result.c_str());
    }
    return 0;
}