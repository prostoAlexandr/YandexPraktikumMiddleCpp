#include <memory>
#include <print>
#include <string>
#include <vector>

class Widget {
public:
    Widget(std::string_view name, Widget *parent) : name_(name), parent_(parent) {}

    // Метод для создания дочернего виджета
    Widget* CreateChild(std::string_view childName) {
        auto child = std::make_unique<Widget>(childName, this);
        children_.push_back(std::move(child));
        return children_.back().get();
    }

    // Метод для отображения иерархии виджетов
    void PrintHierarchy(int level = 0) const {
        const std::string indent(level * 2, ' ');

        if (parent_) {
            std::print("{} - {} (Parent: {})\n", indent, name_, parent_->name_);
        } else {
            std::print("{} - {}", indent, name_);
        }

        for (const auto &child : children_) {
            child->PrintHierarchy(level + 1);
        }
    }

private:
    std::string name_;
    Widget *parent_;                                 // Слабая ссылка на родителя
    std::vector<std::unique_ptr<Widget>> children_;  // Список дочерних виджетов
};

int main() {
    auto root = std::make_unique<Widget>("Root", nullptr);  // Главное окно

    // Создание дочерних виджетов
    auto child1 = root->CreateChild("Child1");
    auto child2 = root->CreateChild("Child2");

    // Создание дочерних виджетов для Child1
    child1->CreateChild("Grandchild1");
    child1->CreateChild("Grandchild2");

    // Вывод иерархии виджетов
    root->PrintHierarchy();

    return 0;
}