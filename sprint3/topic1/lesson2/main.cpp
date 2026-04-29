#include <array>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <ranges>
#include <sstream>
#include <string>
#include <iostream>
struct IPv4 {
    std::array<uint8_t, 4> data = {};
};

bool TryFromString(const std::string_view buf, IPv4 *ipv4) {
    IPv4 tmp;
    auto split_view = buf | std::views::split('.');
    auto it = tmp.data.begin();
    for (auto part : split_view) {
        uint8_t val = {};
        auto result = std::from_chars(part.begin(), part.end(), val);
        if (result.ec != std::errc{})
        {
            return false;
        }
        (*it) = val;
        it = std::next(it);
    }
    std::swap(tmp, *ipv4);
    return true;
}

std::string ToString(const IPv4 &ipv4) {
    std::ostringstream oss;
    for (auto part : ipv4.data) {
        oss << (int)part << '.';
    }
    auto result = oss.str();
    result.pop_back();
    return result;
}

int main()
{
    IPv4 ipv4;
    assert(TryFromString("1.2.3.4", &ipv4));
    std::cout << ToString(ipv4) << std::endl;
    assert(TryFromString("255.254.253.256", &ipv4));
}