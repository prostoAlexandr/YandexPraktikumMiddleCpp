#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <print>
#include <thread>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

using namespace std::literals::chrono_literals;

auto getRandSleep() {
    return [](std::string id) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::println("{}: {}()", time, id);
        std::this_thread::sleep_for(5s);
    };
}

using Function = std::function<void(std::string)>;
using AsyncRes = std::shared_future<void>;
using DeferredRes = std::shared_future<AsyncRes>;

class Task {
public:
    Task(std::string id, Function &&func) : id_(id), func_(std::move(func)) {
        // code here
        fut_ = std::async(std::launch::deferred, [this] {
                   return std::async(std::launch::async, [this] { return run(); }).share();
               }).share();
    }

    DeferredRes getDependency() {
        // code here
        return fut_;
    }

    void addDependency(DeferredRes &&fut) {
        // code here
        deps_.push_back(std::move(fut));
    }

    void run() {
        // code here
        waitCompletion();
        func_(id_);
    }

    void waitCompletion() {
        // code here
        std::vector<AsyncRes> futures;
        for (auto &fut : deps_) {
            if (fut.valid()) {
                futures.push_back(std::move(fut.get()));
            }
        }
        for (auto &fut : futures) {
            fut.wait();
        }
    }

private:
    std::string id_;
    Function func_;
    DeferredRes fut_;
    std::vector<DeferredRes> deps_;
};

struct Item {
    std::string id;
    std::unique_ptr<Task> task;
};

template <class T>
struct Graph {
    std::vector<std::vector<int>> map;
    std::unordered_map<int, T> nodes;
};

template <class T, class Callback>
void bfs(const Graph<T> &g, Callback &&onEdge) {
    for (size_t i = 0; i < g.map.size(); ++i) {
        for (size_t j = 0; j < g.map.at(i).size(); ++j) {
            onEdge(g.nodes.at(i), g.nodes.at(g.map.at(i).at(j)));
        }
    }
}

template <class T>
Task &makePipeline(Graph<T> &g) {
    bfs(g, [](auto &lhs, auto &rhs) {
        // code here
        std::println("{} -> {}", lhs.id, rhs.id);
        lhs.task->addDependency(rhs.task->getDependency());
    });
    return *g.nodes.at(g.map.size() - 1).task;
}

Graph<Item> readYaml(const std::string &config) {
    auto stages = YAML::Load(config);
    Graph<Item> graph;

    std::unordered_map<std::string, int> nodes;
    for (auto it = stages.begin(); it != stages.end(); ++it) {
        auto srcId = it->first.as<std::string>();
        int i = graph.map.size();
        nodes[srcId] = i;
        graph.nodes[i] = Item{srcId, std::make_unique<Task>(srcId, getRandSleep())};
        graph.map.emplace_back();

        if (!it->second["needs"].IsNull()) {
            for (size_t j = 0; j < it->second["needs"].size(); ++j) {
                auto dstId = it->second["needs"][j].as<std::string>();
                graph.map.back().push_back(nodes.at(dstId));
            }
        }
    }
    return graph;
}

int main() {
    std::string cicd = R"(
    start-build:
      stage: build
    build-ios:
      stage: build
      needs:
        - start-build
    build-android:
      stage: build
      needs:
        - start-build
    finish-build:
      stage: build
      needs:
        - build-ios
        - build-android

    start-test:
      stage: test
      needs:
        - finish-build
    test-ios:
      stage: test
      needs:
        - start-test
    test-android:
      stage: test
      needs:
        - start-test
    finish-test:
      stage: test
      needs:
        - test-ios
        - test-android

    start-deploy:
      stage: deploy
      needs:
        - finish-test
    deploy-ios:
      stage: deploy
      needs:
        - start-deploy
    deploy-android:
      stage: deploy
      needs:
        - start-deploy
    finish-deploy:
      stage: deploy
      needs:
        - deploy-ios
        - deploy-android
  )";

    auto g = readYaml(cicd);
    auto &n = makePipeline(g);
    auto fut = n.getDependency();
    fut.get().wait();
    return 0;
}