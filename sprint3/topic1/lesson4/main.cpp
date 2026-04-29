#include <cstdint>
#include <iostream>
#include <vector>

class Player {
public:
    Player(uint32_t number) : number_(number) { std::cout << "created" << std::endl; }
    Player(Player &&other) : number_(std::move(other.number_)) { std::cout << "moved" << std::endl; }

private:
    uint32_t number_;
};

int main() {

    std::vector<Player> players;

    players.emplace_back(1);  // Выведет "created"
    std::cout << std::endl;
    players.push_back({1});  // Выведет "created"
                             //          "moved"
}