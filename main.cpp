#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
    string line;
    int lineCount = 0;
    while (getline(file, line)) {
        ++lineCount;
        cout << lineCount << ". " << line << endl;
    }
    file.close();

    if (lineCount == 0) {
        cout << "No data found in the file." << endl;
        return;
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

    ifstream inputFile(filename);
    ofstream outputFile("temp.txt");

    int currentLine = 0;
    while (getline(inputFile, line)) {
        ++currentLine;
        if (currentLine == studentNum) {
            outputFile << line.substr(0, line.find(',')) << "," << newMarks << endl;
            cout << "Marks updated successfully." << endl;
        } else {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
}

int main() {
    string filename = "marks.txt";

    while (true) {
        cout << "1. Store marks" << endl;
        cout << "2. Update marks" << endl;
        cout << "3. Quit" << endl;

        cout << "Enter your choice: ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            storeMarks(filename);
        } else if (choice == "2") {
            updateMarks(filename);
        } else if (choice == "3") {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "Program exited." << endl;

    return 0;
}
