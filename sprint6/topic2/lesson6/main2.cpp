#include <atomic>
#include <chrono>
#include <thread>

std::atomic<int> last_temperature;

int read_sensor(){
    return 5;
}

void update_sensor() {
    // здесь ваш код
    using namespace std::chrono_literals;
    last_temperature.store(read_sensor(), std::memory_order::relaxed);
    std::this_thread::sleep_for(1s);
}

int get_temperature() {
    // здесь ваш код
    return last_temperature.load(std::memory_order::relaxed);
}

int main() {}