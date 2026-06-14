#include <atomic>

struct DronePosition {
    double x;
    double y;
};
DronePosition GetNextPos(const DronePosition &pos);
void DrawPosition(const DronePosition &pos);

// здесь ваш код
std::atomic<DronePosition> pos;

void motion() {
    while (true) {
        // здесь ваш код
        pos.exchange(GetNextPos(pos));
    }
}

void rendering() {
    // здесь ваш код
}

// struct DronePosition {
//     double x;
//     double y;
// };
// DronePosition GetNextPos(const DronePosition &pos);
// void DrawPosition(const DronePosition &pos);

// std::atomic<DronePosition> pos{{0, 0}};

// void motion() {
//     while (true) {
//         DronePosition curr_pos = GetNextPos(pos.load(std::memory_order_relaxed));
//         pos.store(curr_pos, std::memory_order_relaxed);
//     }
// }

// void rendering() { DrawPosition(pos.load(std::memory_order_relaxed)); }