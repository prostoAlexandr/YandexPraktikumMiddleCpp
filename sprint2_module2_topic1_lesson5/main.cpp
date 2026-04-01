#include <iostream>
#include <memory>

template <typename F, typename G>
class Compose {
    // здесь определите поля структуры
    F _f;
    G _g;

public:
    template <typename F1, typename G1>
    Compose(F1 &&f, G1 &&g) : _f(std::forward<F1>(f)), _g(std::forward<G1>(g)) {
        // здесь ваш код
    }

    // определите operator(), который принимает один шаблонный аргумент и вычисляет композицию функций
    template <typename T>
    decltype(auto) operator()(T &&t) {
        auto &&currency_amount = _g(std::forward<T>(t));
        return _f(currency_amount);
    }
};

template <typename F, typename G>
Compose<F, G> compose(F &&f, G &&g) {
    // здесь ваш код
    return Compose<F, G>(std::forward<F>(f), std::forward<G>(g));
}

int main() {
    auto add_tax = [](int value) { return value + 1; };
    auto convert_currency = [](int value) { return value * 10; };
    auto process_order_price = compose(std::move(add_tax), std::move(convert_currency));
    auto result = process_order_price(5);
    std::cout << "Result is " << result << std::endl;
}