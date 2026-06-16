#include <chrono>
#include <coroutine>
#include <print>
#include <string>
#include <thread>
#include <variant>

// Признак нахождения персонажа
struct Sensor {
    bool player_visible = false;

    bool is_player_visible() const { return player_visible; }
};

// Действия, которые может «выдать» корутина
struct Wait {
    float seconds;
};

struct StateTransition {
    std::string next_state;
};

// Тип возвращаемого значения корутины
using Action = std::variant<Wait, StateTransition>;

// Чтобы использовать в std::visit
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

struct resumable {
    struct promise_type {
        Action action;

        resumable get_return_object() { return resumable{handle_type::from_promise(*this)}; }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        template <typename T>
        std::suspend_always yield_value(T &&value) {
            action = std::forward<T>(value);
            return {};
        }

        void return_void() {}

        void unhandled_exception() { std::terminate(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    handle_type coro;

    resumable() : coro(nullptr) {}
    explicit resumable(handle_type h) : coro(h) {}

    ~resumable() {
        if (coro)
            coro.destroy();
    }

    resumable(const resumable &) = delete;
    resumable &operator=(const resumable &) = delete;

    resumable(resumable &&r) noexcept : coro(r.coro) { r.coro = nullptr; }

    resumable &operator=(resumable &&r) noexcept {
        if (this != &r) {
            if (coro)
                coro.destroy();
            coro = r.coro;
            r.coro = nullptr;
        }
        return *this;
    }

    // Основной метод: возобновить выполнение
    bool resume() {
        if (!coro || coro.done())
            return false;
        coro.resume();
        return true;
    }

    // Получить последнее значение (после co_yield)
    Action get() const { return coro.promise().action; }
};

resumable patrol_state(Sensor &sensor) {
    while (true) {
        std::println("[Patrol] Walking the beat...");
        co_yield Wait{0.5f};

        if (sensor.is_player_visible()) {
            std::println("[Patrol] Player spotted! Switching to Chase.");
            co_yield StateTransition{"chase"};
            co_return;
        }
    }
}
// Логика состояния:
// - Каждые 0.5 с проверяет, виден ли игрок.
// - Если игрок появляется, немедленно переходит в состояние `сhase`.
// - Цикл продолжается до события — нет необходимости во внешнем polling.

resumable chase_state(Sensor &sensor) {
    std::println("[Chase] Running after player!");
    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        co_yield Wait{0.5f};

        if (!sensor.is_player_visible()) {
            auto now = std::chrono::steady_clock::now();
            float elapsed = std::chrono::duration<float>(now - start_time).count();
            if (elapsed >= 5.0f) {
                std::println("[Chase] Lost player. Entering Alert mode.");
                co_yield StateTransition{"alert"};
                co_return;
            }
        } else {
            start_time = std::chrono::steady_clock::now();
        }
    }
}
// Логика состояния:
// - При входе в состояние выводится сообщение.
// - Каждые 0.5 с проверяется, виден ли игрок.
// - Если игрок исчезает, запускается отсчёт 5 с.
// - Если игрок снова появляется, таймер сбрасывается.
// - Если игрок не появляется в течение 5 с, переход в `alert`.

resumable alert_state(Sensor &sensor) {
    std::println("[Alert] Searching for intruder!");
    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        co_yield Wait{0.1f};

        float search_time =
            std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count();

        if (search_time >= 10.0f) {
            std::println("[Alert] No trace found. Resuming patrol.");
            co_yield StateTransition{"patrol"};
            co_return;
        }

        if (sensor.is_player_visible()) {
            std::println("[Alert] Found player! Back to chase!");
            co_yield StateTransition{"chase"};
            co_return;
        }
    }
}
// Логика состояния:
// - Немедленно начинает поиск.
// - Проверяет условия каждые 0.1 с (более частая проверка — активный поиск).
// - Если прошло 10 с без результата, возвращается в `patrol`.
// - Если игрок снова замечен, сразу переходит обратно в `chase`.

class StateMachine {
    std::string current_name;
    resumable current_coro;
    Sensor &sensor;  // Ссылка на сенсор конкретного охранника

    resumable make_coro(const std::string &name) {
        if (name == "patrol")
            return patrol_state(sensor);
        if (name == "chase")
            return chase_state(sensor);
        if (name == "alert")
            return alert_state(sensor);
        throw std::invalid_argument("Unknown state: " + name);
    }

public:
    StateMachine(Sensor &s) : sensor(s) {}

    void set_state(const std::string &name) {
        current_name = name;
        current_coro = make_coro(name);
    }

    std::optional<std::chrono::duration<float>> update() {
        if (!current_coro.resume()) {
            return std::nullopt;  // Корутина завершилась
        }

        Action action = current_coro.get();

        std::optional<std::chrono::duration<float>> sleep_for;
        std::visit(
            Overloaded{[&](const Wait &w) { sleep_for = std::chrono::duration<float>(w.seconds); },
                       [&](const StateTransition &t) {
                           set_state(t.next_state);
                           sleep_for = std::nullopt;  // Переход — не ждём
                       }},
            action);
        return sleep_for;

        return std::nullopt;
    }

    std::string get_state() const { return current_name; }
};

int main() {
    // Охранник 1
    Sensor sensor1;
    StateMachine guard1(sensor1);
    guard1.set_state("patrol");

    // Охранник 2
    Sensor sensor2;
    StateMachine guard2(sensor2);
    guard2.set_state("patrol");

    // Поток для охранника 1
    std::thread t1([&guard1]() {
        for (int i = 0; i < 30; ++i) {
            auto sleep_duration = guard1.update();

            if (sleep_duration.has_value()) {
                std::this_thread::sleep_for(sleep_duration.value());
            }
        }
    });

    // Поток для охранника 2
    std::thread t2([&guard2]() {
        for (int i = 0; i < 30; ++i) {
            auto sleep_duration = guard2.update();
            if (sleep_duration.has_value()) {
                std::this_thread::sleep_for(sleep_duration.value());
            }
        }
    });

    // Симуляция событий
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::println("[EVENT] Player appears near Guard 1");
    sensor1.player_visible = true;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::println("[EVENT] Player disappears");
    sensor1.player_visible = false;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::println("[EVENT] Player appears near Guard 2");
    sensor2.player_visible = true;

    t1.join();
    t2.join();

    return 0;
}