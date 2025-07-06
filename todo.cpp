#include <iostream>
#include <fstream>
#include <sstream>

const std::string SAVE = "/home/halil/Documents/CLI/todolist.txt";

class Todo {
public:
    bool isDone = false;
    std::string title;

    Todo() {
        title = "";
    }

    Todo(std::string title) {
        this->title = "[ ] " + title;
    }

    void check() {
        isDone = true;
        title = title.erase(0, 4);
        title = "[x] \033[9m" + title + "\033[0m";
    }
    void uncheck() {
        isDone = false;
        title = title.erase(0, 8);
        title = title.erase(title.size() - 4, 4);
        title = "[ ] " + title;
    }
    bool empty() {
        return title.empty();
    }
    void print() {
        std::cout << title << std::endl;
    }

};

class Todolist {
private:
    Todo todos[10];
    int count;
public:
    void addTodo(const std::string& title) {
        if (count < 10) {
            todos[count] = Todo(title);
            count++;
        }
        this->printTodos();
    }
    void setTodos(Todo* todos, int count) {
        this->count = count;
        for (int i = 0; i < count; ++i) {
            this->todos[i] = todos[i];
        }

    }

    void removeTodo(int index) {
        if (index >= 0 && index < count) {
            for (int i = index; i < count - 1; ++i) {
                todos[i] = todos[i + 1];
            }
            count--;
        }
        this->printTodos();
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        for (int i = 0; i < count; ++i) {
            file << todos[i].isDone << "|" << todos[i].title << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        count = 0;
        while (std::getline(file, line) && count < 10) {
            std::istringstream iss(line);
            std::string doneStr, title;
            if (std::getline(iss, doneStr, '|') && std::getline(iss, title)) {
                todos[count].isDone = (doneStr == "1");
                todos[count].title = title;
                count++;
            }
        }
    }

    void printTodos() {
        std::cout << "************* TODO LIST *************\n";
        for (int i = 0; i < count; i++) {
            todos[i].print();
        }
        std::cout << "*************************************\n";
    }

    void check(int index) {
        index--;
        if (index >= 0 && index < count) {
            todos[index].check();
            this->printTodos();
        }
        else {
            std::cout << "Invalid index: " << index + 1 << std::endl;
        }
    }
    void uncheck(int index) {
        index--;
        if (index >= 0 && index < count) {
            todos[index].uncheck();
            this->printTodos();
        }
        else {
            std::cout << "Invalid index: " << index + 1 << std::endl;
        }

    };
};

void help();

int main(int argc, char* argv[]) {
    Todolist todolist;
    // Load the todo list from a file
    todolist.loadFromFile(SAVE);
    if (argc < 2) {
        help();
        return 1;
    }
    char option = toupper(argv[1][0]);


    switch (option)
    {
    case 'H':
        help();
        break;
    case 'L':
        todolist.printTodos();
        break;
    case 'A':
        if (argc == 3) {
            todolist.addTodo(argv[2]);
        }
        else {
            std::cout << "Usage: todo add <title>\n";
        }
        break;
    case 'C':
        if (argc == 3) {
            int index = std::stoi(argv[2]);
            todolist.check(index);
        }
        break;
    case 'U':
        if (argc == 3) {
            int index = std::stoi(argv[2]);
            todolist.uncheck(index);
        }
        break;
    case 'R':
        if (argc == 3) {
            int index = std::stoi(argv[2]);
            todolist.removeTodo(index - 1);
        }
        else {
            std::cout << "Usage: todo remove <index>\n";
        }
        break;
    case 'X':
        // Clear the todo list
        todolist.setTodos(nullptr, 0); // Reset the todo list
        todolist.printTodos();
        break;
    default:
        std::cout << "Unknown command: " << argv[1] << "\n";
        help();
        return 1;
    }
    // Save the todo list to a file
    todolist.saveToFile(SAVE);
    return 0;
}

void help() {
    std::cout << "Usage: todo [command] [options]\n";
    std::cout << "Commands:\n";
    std::cout << "  add <title>        Add a new todo item\n";
    std::cout << "  list               List all todo items\n";
    std::cout << "  check <index>      Mark a todo item as done\n";
    std::cout << "  uncheck <index>    Mark a todo item as not done\n";
    std::cout << "  remove <index|all> Remove a todo item\n";
    std::cout << "  help               Show this help message\n";
}