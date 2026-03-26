#include <memory>

template <typename T>
T error_value;

// Here is your code 

// Specialization for pointers:
template <typename T>
T *error_value<T *> = nullptr;

// Specialization for unique_ptr:
template <typename T>
T *error_value<std::unique_ptr<T>> = nullptr;

// Specialization for unique_ptr:
template <typename T>
T *error_value<std::shared_ptr<T>> = nullptr; 

template <>
int error_value<int> = -1;

int main()
{
    auto ec1 = error_value<int*>;
    auto ec2 = error_value<int>;
    auto ec3 = error_value<std::shared_ptr<int>>;
    auto ec4 = error_value<std::unique_ptr<int>>;
}