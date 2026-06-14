#include <string>
#include <atomic>
#include <unordered_set>
#include <memory>

using IP = std::string;
using BannedList = std::unordered_set<IP>;

std::atomic<std::shared_ptr<BannedList>> atomic_ptr;
void SetBannedList(BannedList &&list) {
    // ваш код здесь
    atomic_ptr.store(std::make_shared<BannedList>(std::move(list)));
}

bool IsBanned(const IP &ip) {
    // ваш код здесь
    auto list = atomic_ptr.load();
    return list->contains(ip);
}

int main()
{
    return 0;
}