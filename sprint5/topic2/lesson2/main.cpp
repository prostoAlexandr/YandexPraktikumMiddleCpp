#include <print>
#include <any>
#include <utility>
#include <vector>

struct TextMessage {
};

struct ImageMessage {
};

struct StatusMessage {
};

class MessageQueue {
public:
    //Напишите функцию AddMessage для добавления сообщения в контейнер
    template <typename T>
    void AddMessage(T&& message) {
        messages_.emplace_back(std::forward<T>(message));
    }

    void Show() {
        for(const auto& message: messages_) {
            std::println("{}", message.type().name());
        }
    }
private:
    // Добавьте определение вашего контейнера messages_ тут
    std::vector<std::any> messages_;
};

int main() {
    MessageQueue message_queue;

    message_queue.AddMessage(TextMessage{});
    message_queue.AddMessage(StatusMessage{});
    message_queue.Show();
}
