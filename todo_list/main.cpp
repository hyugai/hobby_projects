#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define PATH "./data.csv"
#define MAX 200

struct Task;
struct ToDoList;

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
    void readTask() {
        ifstream myFile;
        inputFile(myFile);
        inputTasks(myFile, tasks);

        myFile.close();
    }
    void updateTask();
    void deleteTask();
    void displayTask() {
        /*
         * Name
         * | Deadline:
         * | Done:
         * | Description:
         * */
        for (auto i{0}; i < tasks.size(); i++) {
            cout << i + 1 << "." << " ";
            cout << tasks.at(i).getName() << endl;
            cout << "    |Done: " << (tasks.at(i).getIsCompleted() ? "X" : "")
                 << endl;
            if (tasks.at(i).getDeadline().length() != 0) {
                cout << "    |Deadline: " << tasks.at(i).getDeadline() << endl;
            }
            if (tasks.at(i).getDescription().length() != 0) {
                cout << "    |Description: " << tasks.at(i).getDescription()
                     << endl;
            }
        }
    }
    void run();

  private:
    vector<Task> tasks;
};

// read csv file -> load each line to an object of `Task` -> store objects in
// vector
int main() {
    ToDoList myList;
    myList.readTask();

    myList.displayTask();

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
