#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

struct Student {
    string name;
    string marks;
};

void storeMarks(const string& filename) {
    ofstream file(filename, ios::app);
    if (!file) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    cout << "Enter student name (or 'q' to quit): ";
    string name;
    while (getline(cin, name)) {
        if (name == "q")
            break;

        cout << "Enter marks obtained: ";
        string marks;
        getline(cin, marks);

        file << name << "," << marks << endl;

        cout << "Enter student name (or 'q' to quit): ";
    }

    cout << "Data stored successfully." << endl;
    file.close();
}

void updateMarks(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    cout << "Current data:" << endl;
    vector<Student> students;
    Student student;
    int lineCount = 0;
    while (getline(file, student.name, ',')) {
        getline(file, student.marks);
        students.push_back(student);
        ++lineCount;
    }
    file.close();

    if (lineCount == 0) {
        cout << "No data found in the file." << endl;
        return;
    }

    cout << "No.  Name         Marks" << endl;
    cout << "------------------------" << endl;
    for (int i = 0; i < students.size(); ++i) {
        cout << setw(3) << i + 1 << ".  ";
        cout << setw(12) << students[i].name << " ";
        cout << students[i].marks << endl;
    }

    cout << "Enter the number of the student to update marks: ";
    int studentNum;
    cin >> studentNum;
    cin.ignore();

    if (studentNum < 1 || studentNum > lineCount) {
        cout << "Invalid student number." << endl;
        return;
    }

    cout << "Enter the updated marks: ";
    string newMarks;
    getline(cin, newMarks);

    students[studentNum - 1].marks = newMarks;

    ofstream outputFile(filename);
    for (const auto& student : students) {
        outputFile << student.name << "," << student.marks << endl;
    }
    outputFile.close();

    cout << "Marks updated successfully." << endl;
}

void deleteStudent(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    cout << "Current data:" << endl;
    vector<Student> students;
    Student student;
    int lineCount = 0;
    while (getline(file, student.name, ',')) {
        getline(file, student.marks);
        students.push_back(student);
        ++lineCount;
    }
    file.close();

    if (lineCount == 0) {
        cout << "No data found in the file." << endl;
        return;
    }

    cout << "No.  Name         Marks" << endl;
    cout << "------------------------" << endl;
    for (int i = 0; i < students.size(); ++i) {
        cout << setw(3) << i + 1 << ".  ";
        cout << setw(12) << students[i].name << " ";
        cout << students[i].marks << endl;
    }

    cout << "Enter the number of the student to delete: ";
    int studentNum;
    cin >> studentNum;
    cin.ignore();

    if (studentNum < 1 || studentNum > lineCount) {
        cout << "Invalid student number." << endl;
        return;
    }

    students.erase(students.begin() + studentNum - 1);

    ofstream outputFile(filename);
    for (const auto& student : students) {
        outputFile << student.name << "," << student.marks << endl;
    }
    outputFile.close();

    cout << "Student deleted successfully." << endl;
}

int main() {
    string filename = "marks.txt";

    while (true) {
        cout << "1. Store marks" << endl;
        cout << "2. Update marks" << endl;
        cout << "3. Display marks" << endl;
        cout << "4. Delete student" << endl;
        cout << "5. Quit" << endl;

        cout << "Enter your choice: ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            storeMarks(filename);
        } else if (choice == "2") {
            updateMarks(filename);
        } else if (choice == "3") {
            ifstream file(filename);
            if (!file) {
                cout << "Error: Could not open file '" << filename << "'." << endl;
                continue;
            }

            cout << "Marks data:" << endl;
            vector<Student> students;
            Student student;
            while (getline(file, student.name, ',')) {
                getline(file, student.marks);
                students.push_back(student);
            }
            file.close();

            if (students.empty()) {
                cout << "No data found in the file." << endl;
                continue;
            }

            cout << "No.  Name         Marks" << endl;
            cout << "------------------------" << endl;
            for (int i = 0; i < students.size(); ++i) {
                cout << setw(3) << i + 1 << ".  ";
                cout << setw(12) << students[i].name << " ";
                cout << students[i].marks << endl;
            }
        } else if (choice == "4") {
            deleteStudent(filename);
        } else if (choice == "5") {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "Program exited." << endl;

    return 0;
}
