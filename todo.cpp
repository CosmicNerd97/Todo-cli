#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

const std::string SAVE = "/home/halil/Documents/CLI/todolist.txt";

class Todo {
public:
    bool isDone = false;
    std::string title;
    std::string status = "todo";
    std::string description = "";
    std::string checkMark = "[ ]";

    Todo() {
        title = "";
    }

    Todo(std::string title) {
        this->title = title;
    }

    void start() {
        status = "in-progress";
    }
    void check() {
        isDone = true;
        title = "\033[9m" + title + "\033[0m";
        checkMark = "[x]";
        status = "done";
    }
    void uncheck() {
        if (!isDone) return;
        isDone = false;
        title = title.erase(0, 4);
        title = title.erase(title.size() - 4, 4);
        checkMark = "[ ]";
        status = "todo";
    }
    bool empty() {
        return title.empty();
    }
    void print() {
        int totalWidth = 24; // Desired width for the title column
        int titleLen = isDone ? title.length() - 8 : title.length(); // +4 for the check mark and spaces
        int spaces = totalWidth - titleLen;
        if (spaces < 0) spaces = 0;
        std::string padding(spaces, ' ');
        std::cout << "  " << checkMark << "  | " << title << padding << " | " << status << std::endl;
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
        else {
            this->printTodos();
            std::cout << "You have reached the limit.";
            return;
        }
        this->printTodos();

    }

    Todo* getTodos() {
        return this->todos;
    }

    Todo& getTodo(int index) {
        return todos[index];
    }

    void startTodo(int i) {
        getTodo(i).start();
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
            file << todos[i].isDone << "|" << todos[i].title << "|" << todos[i].status << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        count = 0;
        while (std::getline(file, line) && count < 10) {
            std::istringstream iss(line);
            std::string doneStr, title, status;
            if (std::getline(iss, doneStr, '|') && std::getline(iss, title, '|') && std::getline(iss, status)) {
                todos[count].isDone = (doneStr == "1");
                todos[count].checkMark = (doneStr == "1") ? "[x]" : "[ ]";
                todos[count].title = title;
                todos[count].status = status;
                count++;
            }
        }
    }

    void printTodos() {
        std::cout << "\n\033[4m Check | Title                    | Status      \033[0m\n";
        for (int i = 0; i < count; i++) {
            std::cout << "       |                          |             \n";
            todos[i].print();
            std::cout << "\033[4m       |                          |             \033[0m\n";
        }
        std::cout << std::endl;
    }

    void printTodos(std::string filter) {
        std::cout << "\n\033[4m Check | Title                    | Status      \033[0m\n";
        for (int i = 0; i < count; i++) {
            if (todos[i].status == filter) {
                std::cout << "       |                          |             \n";
                todos[i].print();
                std::cout << "\033[4m       |                          |             \033[0m\n";
            }
        }
        std::cout << std::endl;
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
        if (argc == 2) {
            todolist.printTodos();
            break;
        }
        else if (argc == 3) {
            std::string filter = argv[2];
            std::transform(filter.begin(), filter.end(), filter.begin(),
                [](unsigned char c) { return std::tolower(c); });
            if (filter == "done" || filter == "todo" || filter == "in-progress") {
                todolist.printTodos(filter);
            }
            else {
                std::cout << "Usage: todo list <done|todo|in-progress| >";
                break;
            }

        }
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
    case 'E':
        if (argc == 3) {
            int index = std::stoi(argv[2]);
            if (todolist.getTodo(index - 1).empty()) {
                std::cout << "That todo desnt exist\n";
                std::cout << "todo add <title>";
                break;
            }
            if (index > 0 && index <= 10) {
                std::string newTitle;
                std::cout << "Enter new title for todo " << index << ": ";
                std::getline(std::cin, newTitle);
                if (!newTitle.empty()) {
                    todolist.getTodo(index - 1).title = newTitle;
                    todolist.printTodos();
                }
                else {
                    std::cout << "Title cannot be empty.\n";
                }
            }
            else {
                std::cout << "Invalid index: " << index << ". Must be between 1 and 10.\n";
            }
        }
        else {
            std::cout << "Usage: todo edit <index>\n";
            return 1;
        }
        break;
    case 'S':
        if (argc == 3) {
            int i = std::stoi(argv[2]);
            todolist.startTodo(i - 1);
            todolist.printTodos();
        }
        else {
            std::cout << "Usage: todo start <index>";
            return 1;
        }
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
    std::cout << "  edit <index>       Edit todo at the index\n";
    std::cout << "  start <index>      Start the specified todo\n";
    std::cout << "  x                  Clear all todo items\n";
    std::cout << "  help               Show this help message\n";
}