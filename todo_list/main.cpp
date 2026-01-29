#include <cstdlib>
#include <fstream>
#include <ios>
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

void inputMenu(int &);
void inputFile(ifstream &);
void inputTasks(ifstream &, vector<Task> &);
void inputTaskNumber(int &);
void parseOneCsvLine(vector<string> &, const string &);
bool isValidDate(const string &);
bool isCharDigit(const string &, int);
bool isCharDash(const string &, int);

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
    void setDescription(const string &description) {
        this->description = description;
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
    void updateTask(const int &);
    void deleteTask(const int &);

    void displayTasks();
    void showDetails(const int &);
    void markCompleted(const int &);
    void saveTasks();
    void run();

  private:
    vector<Task> tasks;
    // bool flag{false};
};

// read csv file -> load each line to an object of `Task` -> store objects in
// vector
int main() {
    ToDoList myList;
    myList.run();

    return EXIT_SUCCESS;
}
void inputMenu(int &n) {
    string menu{"0.Display 1.Add 2.Update 3.Remove 4.Details 5.Done 6.exit: "};
    cout << "󰮫 Menu:" << endl;
    cout << menu;
    cin >> n;
    // system("clear"); // clear screen
}
void inputFile(ifstream &myFile) {
    myFile.open(PATH);
    if (!myFile) {
        cout << "Failed to open file at " << PATH << endl;
        exit(EXIT_FAILURE);
    }
    // myFile.ignore(MAX, '\n'); // skip first line (header)
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
void inputTaskNumber(int &n) {
    cout << "Enter task's number: ";
    cin >> n;
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
bool isValidDate(const string &date) {
    bool res{true};
    if (date.length() > 10 || date.length() < 0 || !isCharDash(date, 2) ||
        !isCharDash(date, 5) || !isCharDigit(date, 0) ||
        !isCharDigit(date, 1) || !isCharDigit(date, 3) ||
        !isCharDigit(date, 4) || !isCharDigit(date, 6) ||
        !isCharDigit(date, 7) || !isCharDigit(date, 8) ||
        !isCharDigit(date, 9)) {
        res = false;
    }

    return res;
}
bool isCharDigit(const string &date, int n) {
    bool res{true};
    if (date.at(n) > '9' || date.at(n) < '0') {
        res = false;
    }

    return res;
}
bool isCharDash(const string &date, int n) {
    bool res{true};
    if (date.at(n) != '-') {
        res = false;
    }

    return res;
}
void ToDoList::createTask() {
    Task t{"", "01-01-0001", "", 0};
    this->tasks.push_back(t);
    this->updateTask(this->tasks.size());

    this->displayTasks();
}
void ToDoList::readTask() {
    ifstream myFile;
    inputFile(myFile);
    inputTasks(myFile, tasks);

    myFile.close();
}
void ToDoList::updateTask(const int &n) {
    Task &t{this->tasks.at(n - 1)};
    cin.ignore(); // remove \n in buffer

    string inp;
    cout << "Name: ";
    getline(cin, inp); // cin.getline is specifically designed for `char[]`
    if (inp.length()) {
        t.setName(inp);
    }

    cout << "Deadline (dd-MM-yyyy): ";
    getline(cin, inp);
    // add `isValidDate` here
    while (true) {
        if (inp.length() && isValidDate(inp)) {
            t.setDeadline(inp);
            break;
        } else if (inp.length() == 0) { // keep old value unchanged
            break;
        }
        cout << "Invalid date!" << endl << "Enter again (dd-MM-yyyy): ";
        getline(cin, inp);
    }

    cout << "Description: ";
    getline(cin, inp);
    if (inp.length()) {
        t.setDescription(inp);
    }

    cout << "IsCompleted: ";
    getline(cin, inp);
    if (inp.length()) {
        t.setIsCompleted(stoi(inp));
    }

    this->displayTasks();
}
void ToDoList::deleteTask(const int &n) {
    this->tasks.erase(this->tasks.begin() + n - 1);

    this->displayTasks();
}
void ToDoList::displayTasks() {
    system("clear"); // clear screen in Linux and MacOS

    auto maxNameLength{0};
    for (Task t : tasks) {
        maxNameLength =
            (t.getName().length() > maxNameLength ? t.getName().length()
                                                  : maxNameLength);
    }

    // header
    cout << " Tasks: " << endl;
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
void ToDoList::showDetails(const int &n) {
    system("clear"); // clear screen

    Task &t{tasks.at(n - 1)};
    // Name
    cout << "󰋇 " << n << ". " << (t.getIsCompleted() ? STRIKEOUT : "")
         << t.getName() << RESET << endl;
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
void ToDoList::markCompleted(const int &n) {
    Task &t{this->tasks.at(n - 1)};
    t.setIsCompleted(1);

    this->displayTasks();
}
void ToDoList::saveTasks() {
    ofstream myFile{PATH}; // file is truncated if it's read successfully
    if (!myFile) {
        cout << "Failed to open file at " << PATH << endl;
        exit(EXIT_FAILURE);
    }

    for (Task t : tasks) {
        myFile << t.getName() << ',' << t.getDeadline() << ','
               << t.getDescription() << ',' << t.getIsCompleted() << endl;
    }

    myFile.close();
}
void ToDoList::run() {
    this->readTask();
    int choice;
    inputMenu(choice);

    while (true) {
        int n;
        switch ((UserActions)choice) {
        case (UserActions::Display):
            this->displayTasks();
            break;
        case (UserActions::Add):
            this->createTask();
            break;
        case (UserActions::Update):
            inputTaskNumber(n);
            this->updateTask(n);
            break;
        case (UserActions::Remove):
            inputTaskNumber(n);
            this->deleteTask(n);
            break;
        case (UserActions::Details):
            inputTaskNumber(n);
            this->showDetails(n);
            break;
        case (UserActions::Done):
            inputTaskNumber(n);
            this->markCompleted(n);
            break;
        case (UserActions::exit):
            this->saveTasks();
            exit(EXIT_SUCCESS);
        default:
            cout << "Invalid option!" << endl;
            break;
        }
        inputMenu(choice);
    }
}
