#include <cassert>
#include <print>
#include <utility>
#include <vector>

// Класс, реализующий задачу на исполнение
struct Task {
    Task(int id, int *executed) : id(id), executed(executed) {}

    void execute() {
        std::println("execute id={}", id);
        ++(*executed);
    }

    bool done = false;
    int id;
    int *executed;
};

// Класс, реализующий очередь задач
class TaskQ {
public:
    TaskQ() : current(0) {}
    void add(Task &&task) { tasks.push_back(std::move(task)); }
    // Исполняет не более n задач
    void run(int n = 5) {
        for (int i = 0; current != tasks.size() && i < n; ++i, ++current) {
            auto &task = tasks[current];
            try {
                task.execute();
            } catch (...) {
                task.done = true;
            }
        }
    }
    // Очищает очередь задач от исполненных
    void clean() {
        // for (auto begin = tasks.begin(); begin != tasks.end(); ++begin) {
        //   if (begin->done) {
        //     tasks.erase(begin);
        //   }
        // }
        auto iter = std::remove_if(tasks.begin(), tasks.end(), [](const auto &task) { return task.done; });
        tasks.erase(iter, tasks.end());
        current = 0;
    }

private:
    std::vector<Task> tasks;
    int current;
};

int main() {
    {
        TaskQ q;
        q.run();
    }
    {
        TaskQ q;
        q.run();
        q.run();
    }
    {
        int executed = 0;
        TaskQ q;
        for (int i = 0; i < 6; ++i) {
            q.add(Task{i, &executed});
        }
        q.run();
        q.run();
        assert(executed == 6);
    }
    {
        int executed = 0;
        TaskQ q;
        for (int i = 0; i < 10; ++i) {
            q.add(Task{i, &executed});
        }
        q.run();
        q.clean();
        q.run();
        assert(executed == 10);
    }
}