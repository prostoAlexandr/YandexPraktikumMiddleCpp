#include <string>
#include <variant>
#include <print>

void Print(std::variant<std::string, double, int> v){
    if (std::holds_alternative<std::string>(v))
    {
        std::println("{}", "Получен std::string");
    }
    else if (std::holds_alternative<double>(v))
    {
        std::println("{}", "Получен double");
    }
    else if (std::holds_alternative<int>(v))
    {
        std::println("{}", "Получен int");
    }
}

void Print(std::variant<std::string, double, std::string> v)
{
    if (v.index() == 0) {
        std::println("Получена строка {}", std::get<0>(v));
    } else if (v.index() == 1) {
        std::println("Получено число {}", std::get<1>(v));
    } else if (v.index() == 2) {
        std::println("Получена строка {}", std::get<2>(v));
    }
}

int main(){
    std::variant<std::string, double, int> v;
}