#include <chrono>
#include <mutex>
#include <print>
#include <thread>

enum class GuardState { Patrol, Chase, Alert };

class Guard {
    GuardState state = GuardState::Patrol;
    std::chrono::steady_clock::time_point state_start_time;

    // Мьютекс для защиты состояния при многопоточном доступе
    mutable std::mutex mtx;

    bool player_visible = false;
    float dt = 0.1f;  // Имитация delta time

public:
    void set_player_visibility(bool visible);
    void update();  // Вызывается из game loop
    std::string get_state() const;
};
void Guard::set_player_visibility(bool visible) {
    std::lock_guard<std::mutex> lock(mtx);
    player_visible = visible;
}

std::string Guard::get_state() const {
    std::lock_guard<std::mutex> lock(mtx);
    switch (state) {
    case GuardState::Patrol:
        return "Patrol";
    case GuardState::Chase:
        return "Chase";
    case GuardState::Alert:
        return "Alert";
    }
    return "Unknown";
}

void Guard::update() {
    std::lock_guard<std::mutex> lock(mtx);  // Блокировка на весь update!

    auto now = std::chrono::steady_clock::now();

    switch (state) {
    case GuardState::Patrol:
        if (player_visible) {
            state = GuardState::Chase;
            state_start_time = now;
        }
        break;

    case GuardState::Chase:
        if (!player_visible) {
            float elapsed = std::chrono::duration<float>(now - state_start_time).count();
            if (elapsed >= 5.0f) {
                state = GuardState::Alert;
                state_start_time = now;
            }
        } else {
            state_start_time = now;
        }
        break;

    case GuardState::Alert:
        float search_time = std::chrono::duration<float>(now - state_start_time).count();
        if (search_time >= 10.0f) {
            state = GuardState::Patrol;
        }
        if (player_visible) {
            state = GuardState::Chase;
            state_start_time = now;
        }
        break;
    }
}
int main() {
    Guard guard1, guard2;

    auto run_guard = [](Guard &g) {
        for (int i = 0; i < 100; ++i) {
            g.update();
            std::println("{}", g.get_state());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    std::thread t1(run_guard, std::ref(guard1));
    std::thread t2(run_guard, std::ref(guard2));

    // Симуляция события: игрок появился
    std::this_thread::sleep_for(std::chrono::seconds(1));
    guard1.set_player_visibility(true);
    // Симуляция события: игрок пропал из виду
    std::this_thread::sleep_for(std::chrono::seconds(1));
    guard1.set_player_visibility(false);

    t1.join();
    t2.join();

    return 0;
}