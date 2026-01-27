#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define PATH "./data.csv"
#define MAX 200
#define RESET "\e[0m"
// this code is right, but github_dark_colorblind fucked the GREEN
#define GREEN "\e[38;5;47m"
#define BLUE "\e[38;5;33m"
#define YELLOW "\e[38;5;226m"
#define RED "\e[38;5;196m"
#define STRIKEOUT "\e[9m"
#define COLUMNSEPARATOR " | "

enum class UserActions { Display, Add, Update, Remove, Details, Done, exit };
struct Task;
struct ToDoList;

void input(int &);
void inputFile(ifstream &);
void inputTasks(ifstream &, vector<Task> &);
void parseOneCsvLine(vector<string> &, const string &);

struct Task {
    // string variables can be `lvalue`
    Task(const string &name, const string &deadline, const string &description,
         const int &isCompleted) {
        this->name = name;
        this->deadline = deadline;
        this->description = description;
        this->isCompleted = isCompleted;
    }

    // why is `const` used here? -> Ans: read-only methods
    string getName() const { return name; }
    string getDeadline() const { return deadline; }
    string getDescription() const { return description; }
    int getIsCompleted() const { return isCompleted; }

    // why is `const` used here? -> Ans: pass by reference to avoid copying
    // large object & read-only parameter
    void setName(const string &name) { this->name = name; }
    void setDeadline(const string &deadline) { this->deadline = deadline; }
    void setDescription(const string &deadline) {
        this->deadline = description;
    }
    void setIsCompleted(const int &isCompleted) {
        this->isCompleted = isCompleted;
    }

  private:
    string name, deadline, description;
    int isCompleted;
};
struct ToDoList {
    void createTask();
    void readTask();
    void updateTask();
    void deleteTask();

    void displayTask();
    void showDetails();
    void markCompleted();
    void run();

  private:
    vector<Task> tasks;
};

// read csv file -> load each line to an object of `Task` -> store objects in
// vector
int main() {
    ToDoList myList;
    myList.run();

    return EXIT_SUCCESS;
}

void inputFile(ifstream &myFile) {
    myFile.open(PATH);
    if (!myFile) {
        cout << "Failed to open file at " << PATH << endl;
        exit(EXIT_FAILURE);
    }
    myFile.ignore(MAX, '\n'); // skip first line (header)
}
void inputTasks(ifstream &myFile, vector<Task> &tasks) {
    auto i{0};
    string line;
    while (getline(myFile, line)) {
        // parse line into elements & store them in `record`
        vector<string> record;
        parseOneCsvLine(record, line);

        tasks.push_back(
            Task{record.at(0), record.at(1), record.at(2),
                 stoi(record.at(3))}); // explicitly cast string to interger
    }
}
void parseOneCsvLine(vector<string> &record, const string &line) {
    auto i{0};
    do {
        auto next_i{line.find(',', i)};
        auto length{(next_i == -1 ? line.length() : next_i) - i};
        record.push_back(line.substr(i, length));

        i = next_i + 1;
    } while (i != 0);
}
void ToDoList::readTask() {
    ifstream myFile;
    inputFile(myFile);
    inputTasks(myFile, tasks);

    myFile.close();
}
void ToDoList::showDetails() {
    cout << "Enter task's number: ";
    int i;
    cin >> i;
    Task &t{tasks.at(i - 1)};
    // Name
    cout << "󰋇 " << (t.getIsCompleted() ? STRIKEOUT : "") << t.getName()
         << RESET << endl;
    // Deadline
    cout << YELLOW " 󰃭 " << t.getDeadline() << RESET << endl;
    // IsCompleted
    cout << (t.getIsCompleted() ? GREEN "  (completed)" RESET
                                : RED "  (pending)" RESET)
         << endl;
    // Description
    cout << BLUE "  "
         << (t.getDescription().length() ? t.getDescription() : "") << RESET
         << endl;
}
void ToDoList::displayTask() {
    auto maxNameLength{0};
    for (Task t : tasks) {
        maxNameLength =
            (t.getName().length() > maxNameLength ? t.getName().length()
                                                  : maxNameLength);
    }

    // header
    cout << "i." << string((tasks.size() > 9 ? 2 : 1), ' ') << "Name"
         << string(maxNameLength - 4, ' ') << COLUMNSEPARATOR; // Name
    cout << "Deadline" << string(2, ' ') << COLUMNSEPARATOR;   // Due
    cout << "Completed" << COLUMNSEPARATOR << endl;            // Deadline
    cout << string(maxNameLength + 30, '+') << endl;

    // rows
    for (auto i{0}; i < tasks.size(); i++) {
        Task &t{tasks.at(i)};
        cout << (t.getIsCompleted() ? STRIKEOUT : "");
        cout << i + 1 << ". " << t.getName()
             << string(maxNameLength - t.getName().length(), ' ')
             << COLUMNSEPARATOR;                             // Name
        cout << t.getDeadline() << COLUMNSEPARATOR << RESET; // Due
        cout << string(4, ' ') << (t.getIsCompleted() ? GREEN "" : RED "")
             << RESET << string(4, ' ') << COLUMNSEPARATOR << endl; // Completed
        cout << string(maxNameLength + 30, '-') << endl;
    }
}
void input(int &n) {
    string menu{"0.Display 1.Add 2.Update 3.Remove 4.Details 5.Done 6.exit: "};
    cout << menu;
    cin >> n;
    // system("clear"); // clear screen
}
void ToDoList::run() {
    this->readTask();
    int choice;
    input(choice);

    while (true) {
        switch ((UserActions)choice) {
        case (UserActions::Display):
            this->displayTask();
            break;
        case (UserActions::Add):
            // this->createTask();
            break;
        case (UserActions::Update):
            // this->updateTask();
            break;
        case (UserActions::Remove):
            this->deleteTask();
            break;
        case (UserActions::Details):
            this->showDetails();
            break;
        case (UserActions::Done):
            this->markCompleted();
            break;
        case (UserActions::exit):
            exit(EXIT_SUCCESS);
        default:
            cout << "Invalid option!" << endl;
            break;
        }
        input(choice);
    }
}
void ToDoList::markCompleted() {}
