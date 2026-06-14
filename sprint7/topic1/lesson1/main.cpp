#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <strings.h>
#include <thread>

#include <arpa/inet.h>
#include <errno.h>
#include <resolv.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <unistd.h>

using namespace std::literals::chrono_literals;

constexpr int MAXBUF = 4;
constexpr int MAX_EPOLL_EVENTS = 64;
constexpr int TIMEOUT_1S = 1;

static void closeFd(int *sd) {
    if (!sd || *sd < 0) {
        return;
    }
    close(*sd);
}

auto timeSpecFor500ms() {
    struct itimerspec timer;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000;  // 0.5s
    timer.it_value = ts;
    timer.it_interval = ts;
    return timer;
}

class Client {
public:
    Client()
        : epfd(new int(-1), &closeFd), sockfd(new int(-1), &closeFd),
          timerfd(new int(-1), &closeFd), connected(false) {
        *sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        *epfd = epoll_create(1);
        *timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

        auto timer = timeSpecFor500ms();
        timerfd_settime(*timerfd, 0, &timer, nullptr);

        socketEvent.events = EPOLLIN;
        socketEvent.data.fd = *sockfd;
        epoll_ctl(*epfd, EPOLL_CTL_ADD, *sockfd, &socketEvent);

        timerEvent.events = EPOLLIN;
        timerEvent.data.fd = *timerfd;
        epoll_ctl(*epfd, EPOLL_CTL_ADD, *timerfd, &timerEvent);
    }

    std::string fetch(const std::string &host, int port) {
        resolve(host, port);

        if (::connect(*sockfd, reinterpret_cast<sockaddr *>(&destination), sizeof(destination)) !=
            0) {
            if (errno != EINPROGRESS) {
                throw std::runtime_error("Unable to connect: " + std::to_string(errno));
            }
        }

        handleEvents();

        return response;
    }

private:
    void handleEvents() {
        int numReady = -1;
        while (numReady != 0) {
            numReady = epoll_wait(*epfd, events, MAX_EPOLL_EVENTS, TIMEOUT_1S);
            for (int i = 0; i < numReady; i++) {
                if (events[i].events & EPOLLIN) {
                    if (events[i].data.fd == *timerfd) {
                        uint64_t _;
                        ::read(*timerfd, &_, sizeof(uint64_t));
                        onTimer();
                        continue;
                    }
                    if (connected) {
                        auto buf = read();
                        onData(std::move(buf));
                    } else {
                        onConnect();
                    }
                    std::this_thread::sleep_for(500ms);
                }
            }
        }
    }

    void resolve(const std::string &host, int port) {
        bzero(&destination, sizeof(destination));
        destination.sin_family = AF_INET;
        destination.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &destination.sin_addr.s_addr) == 0) {
            throw std::runtime_error("Unable to resolve address: " + std::to_string(errno));
        }
    }

    std::string read() {
        std::string tmp;
        tmp.resize(MAXBUF);

        auto sz = recv(*sockfd, tmp.data(), tmp.size(), 0);

        tmp.resize(sz);
        return tmp;
    }

    void onConnect() {
        std::println("Connected");
        connected = true;
    }

    void onData(std::string &&buf) {
        std::println("Got: '{}'", buf);
        response += buf;
    }

    void onTimer() { std::println("Timer"); }

private:
    std::unique_ptr<int, decltype(&closeFd)> epfd;
    std::unique_ptr<int, decltype(&closeFd)> sockfd;
    std::unique_ptr<int, decltype(&closeFd)> timerfd;
    epoll_event socketEvent;
    epoll_event timerEvent;
    epoll_event events[MAX_EPOLL_EVENTS];
    sockaddr_in destination;
    bool connected;
    std::string response;
};

int main() {
    Client client;
    auto res = client.fetch("127.0.0.1", 22);
    std::println("{}", res);
    assert(res == "SSH-2.0-OpenSSH_8.9p1 Ubuntu-3ubuntu0.13");
    return 0;
}