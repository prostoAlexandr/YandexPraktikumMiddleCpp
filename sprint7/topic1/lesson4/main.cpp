#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <print>

using namespace std::literals::chrono_literals;

struct Config {
    std::string name;
    int threadCount;
};

class Application : public std::enable_shared_from_this<Application> {
public:
    void initialize(std::istream &stream) {
        std::promise<void> configReady;
        // code here
        auto shared_future = configReady.get_future().share();

        std::thread thread1([=, this_ = shared_from_this()]() {
            // code here
            shared_future.wait();
            this_->initializeGui();
        });
        thread1.detach();

        std::thread thread2([=, this_ = shared_from_this()]() {
            // code here
            shared_future.wait();
            this_->initializeLogging();
        });
        thread2.detach();

        config_ = read(stream);
        configReady.set_value();
    }

private:
    Config read(std::istream &stream) {
        stream >> config_.name;
        stream >> config_.threadCount;
        return {};
    }
    void initializeGui() { std::println("initializeGui"); }
    void initializeLogging() { std::println("initializeLogging"); }

private:
    Config config_;
};

int main() {
    auto application = std::make_shared<Application>();
    application->initialize(std::cin);
    std::this_thread::sleep_for(1s);
}