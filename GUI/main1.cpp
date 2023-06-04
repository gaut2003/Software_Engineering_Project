#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <json/json.h>

using namespace std;
using namespace cppcms;
using namespace Json;

class Student {
private:
    string name;
    string marks;

public:
    Student(const string& name, const string& marks) : name(name), marks(marks) {}

    string getName() const {
        return name;
    }

    string getMarks() const {
        return marks;
    }

    void setMarks(const string& newMarks) {
        marks = newMarks;
    }
};

class StudentRecordApp : public application {
private:
    vector<Student> students;
    mutex studentsMutex;

public:
    StudentRecordApp(cppcms::service& srv) : application(srv) {
        dispatcher().assign("/store", &StudentRecordApp::storeMarks, this);
        dispatcher().assign("/update", &StudentRecordApp::updateMarks, this);
        dispatcher().assign("/delete", &StudentRecordApp::deleteStudent, this);
        dispatcher().assign("/fetch", &StudentRecordApp::fetchRecords, this);
    }

    void storeMarks() {
        response().content_type("application/json");
        http::request& req = request();
        string name = req.post("name");
        string marks = req.post("marks");

        lock_guard<mutex> lock(studentsMutex);
        students.emplace_back(name, marks);

        response().out() << "{ \"success\": true }";
    }

    void updateMarks() {
        response().content_type("application/json");
        http::request& req = request();
        int studentNum = stoi(req.post("studentNum")) - 1;
        string newMarks = req.post("newMarks");

        lock_guard<mutex> lock(studentsMutex);
        if (studentNum >= 0 && studentNum < students.size()) {
            students[studentNum].setMarks(newMarks);
            response().out() << "{ \"success\": true }";
        } else {
            response().out() << "{ \"success\": false, \"message\": \"Invalid student number.\" }";
        }
    }

    void deleteStudent() {
        response().content_type("application/json");
        http::request& req = request();
        int studentNum = stoi(req.post("studentNum")) - 1;

        lock_guard<mutex> lock(studentsMutex);
        if (studentNum >= 0 && studentNum < students.size()) {
            students.erase(students.begin() + studentNum);
            response().out() << "{ \"success\": true }";
        } else {
            response().out() << "{ \"success\": false, \"message\": \"Invalid student number.\" }";
        }
    }

    void fetchRecords() {
        response().content_type("application/json");

        Value responseJSON;
        Value recordsJSON(arrayValue);

        lock_guard<mutex> lock(studentsMutex);
        for (const auto& student : students) {
            Value studentJSON(objectValue);
            studentJSON["name"] = student.getName();
            studentJSON["marks"] = student.getMarks();
            recordsJSON.append(studentJSON);
        }

        responseJSON["success"] = true;
        responseJSON["records"] = recordsJSON;

        StyledWriter writer;
        response().out() << writer.write(responseJSON);
    }
};

int main(int argc, char** argv) {
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<StudentRecordApp>());
        srv.run();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
