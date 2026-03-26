#include <cmath>
#include <iostream>

template <typename T1, typename T2>
bool is_equal(const T1 &a, const T2 &b) {
    return a == b;
}

template <typename T1, typename T2>
bool is_equal(T1 *a, T2 *b) {
    return *a == *b;
}

bool is_equal(double a, double b, double epsilon) { return std::abs(a - b) < epsilon; }

bool is_equal(double a, double b) { return is_equal(a, b, 1e-7); }

bool is_equal(float a, double b) { return is_equal(static_cast<double>(a), b, 1e-5); }
bool is_equal(double a, float b) { return is_equal(a, static_cast<double>(b), 1e-5); }
bool is_equal(float a, float b) { return is_equal(static_cast<double>(a), static_cast<double>(b), 1e-5); } 

#define STRINGIZE(x) "Value of " << #x << " Is " << x

int main()
{
    int val1 = 5;
    int val2 = 5;
    int val3 = 6;
    std::cout << STRINGIZE(is_equal(val1, val2)) << std::endl <<
        STRINGIZE(is_equal(val2, val3)) << std::endl <<
        STRINGIZE(is_equal(&val1, &val3)) << std::endl <<
        STRINGIZE(is_equal(&val1, &val1)) << std::endl <<
        STRINGIZE(is_equal(&val1, &val2)) << std::endl <<
        STRINGIZE(is_equal(0.5, 0.51)) << std::endl <<
        STRINGIZE(is_equal(0.5, 0.5000001)) << std::endl;
}

// Value of is_equal(val1, val2) Is 1
// Value of is_equal(val2, val3) Is 0
// Value of is_equal(&val1, &val3) Is 0
// Value of is_equal(&val1, &val1) Is 1
// Value of is_equal(&val1, &val2) Is 1
// Value of is_equal(0.5, 0.51) Is 0
// Value of is_equal(0.5, 0.5000001) Is 1
