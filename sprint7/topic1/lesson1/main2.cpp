#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <print>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

constexpr int MAXBUF = 8;
constexpr int MAX_EPOLL_EVENTS = 64;
constexpr int TIMEOUT = -1;
constexpr int BACKLOG = 5;

static void myclose(int* sd) {
  if (!sd || *sd < 0) {
    return;
  }
  close(*sd);
}

class EchoServer {
public:
  EchoServer()
    : epfd(new int(-1), &myclose)
    , sockfd(new int(-1), &myclose)
  {
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    *epfd = epoll_create(1);
    event.events = EPOLLIN;
    event.data.fd = *sockfd;
    epoll_ctl(*epfd, EPOLL_CTL_ADD, *sockfd, &event);
  }

  std::string run(const std::string& host, int port) {
    resolve(host, port);

    if (bind(*sockfd, reinterpret_cast<sockaddr*>(&destination), sizeof(destination)) != 0) {
        throw std::runtime_error("Unable to bind: " + std::to_string(errno));
    }

    if (listen(*sockfd, BACKLOG) == -1) {
        throw std::runtime_error("Failed on listen: " + std::to_string(errno));
    }

    handleEvents();

    return response;
  }

private:
  void handleEvents() {
    int numReady = -1;
    while (numReady != 0) {
      numReady = epoll_wait(*epfd, events, MAX_EPOLL_EVENTS, TIMEOUT);
      for (int i = 0; i < numReady; i++) {
        if (events[i].data.fd == *sockfd) {
          int fd = accept(*sockfd, nullptr, nullptr);
          if (fd == -1) {
            continue;
          }
          event.events = EPOLLIN;
          event.data.fd = fd;
          epoll_ctl(*epfd, EPOLL_CTL_ADD, fd, &event);
          onConnection(fd);
        } else {
          char buf[MAXBUF];
          int fd = events[i].data.fd;
          int sz = recv(fd, buf, MAXBUF, 0);
          if (sz > 0) {
            onReceived(fd, {buf, static_cast<size_t>(sz)});
          } else {
            onFinish(fd);
          }
        }
      }
    }
  }

  void resolve(const std::string& host, int port) {
    bzero(&destination, sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &destination.sin_addr.s_addr) == 0) {
      throw std::runtime_error("Unable to resolve address: " + std::to_string(errno));
    }
  }

  void onConnection(int fd) {
    auto& [_, sent] = fds_[fd];
    sent = false;
    std::println("{} onConnection", fd);
  }

  void onReceived(int fd, std::string_view buf) {
    std::println("{} onReceived: {}", fd, buf);

    auto& [buffer, sent] = fds_[fd];
    buffer += buf;
    if (!sent && buf.size() > 0 && buf.ends_with('\n')) {
      onRequestReady(fd);
      onFinish(fd);
    }
  }

  void onRequestReady(int fd) {
    auto& [buf, sent] = fds_[fd];
    send(fd, buf.data(), buf.size(), 0);
    sent = true;
    std::println("{} onRequestReady", fd);
  }

  void onFinish(int fd) {
    if (fds_.contains(fd)) {
      close(fd);
      epoll_ctl(*epfd, EPOLL_CTL_DEL, fd, nullptr);
      fds_.erase(fd);
    }
    std::println("{} onFinish", fd);
  }

private:
  std::unique_ptr<int, decltype(&myclose)> epfd;
  std::unique_ptr<int, decltype(&myclose)> sockfd;
  epoll_event event;
  epoll_event events[MAX_EPOLL_EVENTS];
  sockaddr_in destination;
  std::string response;
  std::unordered_map<int, std::pair<std::string, bool>> fds_;
};

int main() {
    EchoServer server;
    server.run("127.0.0.1", 8080);
    return 0;
}