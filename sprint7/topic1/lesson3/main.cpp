#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <optional>
#include <print>
#include <stop_token>
#include <thread>

using namespace std::literals::chrono_literals;

using Guid = std::string;

Guid genGuid() {
    static int n = 0;
    return "0-1-2-" + std::to_string(++n);
}

auto now() { return std::chrono::system_clock::now().time_since_epoch().count(); }

class Lease {
public:
    Lease() : guid_(genGuid()) {}
    void prolongate() {
        std::this_thread::sleep_for(100ms);
        std::println("[{}] prolongate until: {}", guid_, now() + 5);
    }
    Guid getId() const { return guid_; }

private:
    Guid guid_;
};

class Transaction {
public:
    ~Transaction() {
        // your code here
        if (guid_) {
            finished_.set_value();
        }
    }
    void start() {
        if (guid_) {
            return;
        }
        Lease lease;
        guid_ = lease.getId();
        // your code here
        auto th = std::jthread([lease, future = finished_.get_future()](std::stop_token st) mutable {
            while (future.wait_for(500ms) != std::future_status::ready) {
                lease.prolongate();
            }
        });
        th.detach();
        std::println("[{}] Tx started", *guid_);
    }

    void commit() {
        if (!guid_) {
            throw std::logic_error("Start first");
        }
        std::println("[{}] Tx commited", *guid_);
        finished_.set_value();
        guid_ = std::nullopt;
    }

    Guid getId() const {
        if (!guid_) {
            throw std::logic_error("Start first");
        }
        return *guid_;
    }

private:
    std::optional<Guid> guid_;
    // your code here
    std::promise<void> finished_;
};

class TableManager {
public:
    Transaction createTransaction() { return {}; }

    Guid createTable(const std::string &path, const Transaction &tx) {
        std::this_thread::sleep_for(3s);
        auto guid = genGuid();
        std::println("Create table {}, guid: {}, under tx: {}", path, guid, tx.getId());
        return guid;
    }
};

int main() {
    {
        TableManager tableManager;
        auto transaction = tableManager.createTransaction();
        transaction.start();
        tableManager.createTable("//home/table1", transaction);
        tableManager.createTable("//home/table2", transaction);
        transaction.commit();
    }
    return 0;
}