#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define PATH "./data.csv"
#define MAX 200
#define COLORSTART "\033["
#define COLOREND "\033[0m"
#define COLORGREEN "32m"

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
    int i;
    cout << "Enter task's number: " << endl;
    cin >> i;
    cout << i << "." << " ";
    cout << tasks.at(i - 1).getName() << endl;
    cout << "    |Done: " << (tasks.at(i - 1).getIsCompleted() ? "X" : "")
         << endl;
    if (tasks.at(i - 1).getDeadline().length() != 0) {
        cout << "    |Deadline: " << tasks.at(i - 1).getDeadline() << endl;
    }
    if (tasks.at(i - 1).getDescription().length() != 0) {
        cout << "    |Description: " << tasks.at(i).getDescription() << endl;
    }
}
void ToDoList::displayTask() {
    auto maxNameLength{0};
    for (Task t : tasks) {
        maxNameLength =
            (t.getName().length() > maxNameLength ? t.getName().length()
                                                  : maxNameLength);
    }

    cout << "Name" << string(maxNameLength, ' ') << "Due" << string(9, ' ')
         << "Done" << endl;
    cout << string(maxNameLength + 20, '+') << endl;

    for (Task t : tasks) {
        cout << t.getName() // Name
             << string(maxNameLength - t.getName().length() + 4, ' ');
        cout << t.getDeadline() << string(4, ' ');       // Due
        cout << (t.getIsCompleted() ? "x" : "") << endl; // Done
        cout << string(maxNameLength + 20, '-') << endl;
    }
}
void input(int &n) {
    string menu{"0.Display 1.Add 2.Update 3.Remove 4.Details 5.Done 6.exit : "};
    cout << COLORSTART << COLORGREEN << menu << COLOREND;
    cin >> n;
    system("clear"); // clear screen
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
            break;
        case (UserActions::Update):
            break;
        case (UserActions::Remove):
            break;
        case (UserActions::Details):
            break;
        case (UserActions::Done):
        case (UserActions::exit):
            exit(EXIT_SUCCESS);
        default:
            cout << "Invalid option!" << endl;
            break;
        }
        input(choice);
    }
}
