## Использование функции std::exchange
```cpp
void ExecuteTasks() {
    MyVector<Task> tasksToExecute{};
    using std::swap;  // Добавляем определение `std::swap`

    swap(tasksToExecute, tasks_);

    for (const auto &task : tasksToExecute) {
        std::invoke(task);
    }
}

void ExecuteTasks() {
    MyVector<Task> tasksToExecute = std::exchange(tasks_, {});

    for (const auto &task : tasksToExecute) {
        std::invoke(task);
    }
}

void ExecuteTasks() {
    for (const auto &task : std::exchange(tasks_, {})) {
        std::invoke(task);
    }
}
```