#include <coroutine>
#include <filesystem>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace fs = std::filesystem;

struct url {
    std::string scheme;
    std::string path;
};

struct LocalFile {
    fs::path path;
    static LocalFile open(const std::string &p) {
        std::println("Open local file {}", fs::path(p).string());
        return {fs::path(p)};
    }
};

struct RemoteFile {
    fs::path tmp_path;
    static RemoteFile open(url u) {
        std::println("Open remote file {}", u.path);
        return {fs::temp_directory_path() / std::string(u.path + "_remote.tmp")};
    }
    void close() {
        fs::remove(tmp_path);
        std::println("Close remote file {}", tmp_path.string());
    }
};

struct CachedFile {
    fs::path cache_path;
    static CachedFile open(url u) {
        std::println("Open cached file {}", u.path);
        return {fs::temp_directory_path() / std::string(u.path + "_cache.bin")};
    }
    void close() {
        fs::remove(cache_path);
        std::println("Close cached file{}", cache_path.string());
    }
};

std::string read_file(const fs::path &p) {
    // Имитация чтения
    std::println("read_file {}", p.filename().string());
    if (p.filename().string().contains("exception")) {
        throw std::runtime_error("Read failed");
    }
    return "content";
}

template <typename T>
struct promise_type {
    const T *yielded_value = nullptr;
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() noexcept {}
    void unhandled_exception() { throw; }
    std::suspend_always yield_value(const T &val) noexcept {
        yielded_value = &val;
        return {};
    }
    auto get_return_object() { return std::coroutine_handle<promise_type>::from_promise(*this); }
};

template <typename T>
class co_resource {
public:
    using promise_type = ::promise_type<T>;
    using handle_type = std::coroutine_handle<promise_type>;

    co_resource(handle_type coro) : coro_(coro) {}
    co_resource(const co_resource &) = delete;
    co_resource &operator=(const co_resource &) = delete;

    co_resource(co_resource &&from) : coro_(std::exchange(from.coro_, nullptr)) {}

    co_resource &operator=(co_resource &&from) {
        std::destroy_at(this);
        std::construct_at(this, std::move(from));
        return *this;
    }

    ~co_resource() {
        if (!coro_)
            return;
        coro_.resume();   // ← Выполняем код после co_yield!
        coro_.destroy();  // ← Теперь уничтожаем корутину
    }

    const fs::path &operator*() { return *coro_.promise().yielded_value; }

private:
    handle_type coro_;
};

co_resource<fs::path> acquire_local_path(url u) {
    if (u.scheme == "file") {
        auto lf = LocalFile::open(u.path);
        co_yield lf.path;
        // Ничего не освобождаем
    } else if (u.scheme == "http") {
        auto rf = RemoteFile::open(u);
        co_yield rf.tmp_path;
        rf.close();
    } else if (u.scheme == "cache") {
        auto cf = CachedFile::open(u);
        co_yield cf.cache_path;
        cf.close();
    } else {
        throw std::invalid_argument("Unsupported URL scheme");
    }
}

// Пример работы корутин
int main() {
    try {
        auto coro1 = acquire_local_path({"http", "data_http"});
        auto coro2 = acquire_local_path({"file", "data_file"});
        auto coro3 = acquire_local_path({"cache", "data_cache"});
        auto coro4 = acquire_local_path({"http", "exception"});
        // Работаем с файлом
        std::println("File content {}", read_file(*coro1));
        std::println("File content {}", read_file(*coro2));
        std::println("File content {}", read_file(*coro3));
        std::println("File content {}", read_file(*coro4));

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        // Даже здесь — ресурсы освобождены!
    }
}