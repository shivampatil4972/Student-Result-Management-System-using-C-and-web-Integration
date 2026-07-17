#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

class Date {
public:
    int day, month, year;
    Date() : day(1), month(1), year(2000) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    string toString() const {
        return (day < 10 ? "0" : "") + to_string(day) + "/" +
               (month < 10 ? "0" : "") + to_string(month) + "/" + to_string(year);
    }
};

class Subject {
public:
    string name;
    float marks;
    float maxMarks;
    Subject() : name(""), marks(0), maxMarks(100) {}
    Subject(string n, float m, float max = 100) : name(n), marks(m), maxMarks(max) {}
};

class Result {
public:
    float totalMarks = 0;
    float percentage = 0;
    string grade = "-";
    string status = "PENDING";

    void calculate(const vector<Subject>& subjects) {
        totalMarks = 0;
        for (const auto& sub : subjects) totalMarks += sub.marks;
        percentage = subjects.empty() ? 0 : (totalMarks / subjects.size());

        if (percentage >= 80) { grade = "A+"; status = "PASS"; }
        else if (percentage >= 70) { grade = "A"; status = "PASS"; }
        else if (percentage >= 60) { grade = "B"; status = "PASS"; }
        else if (percentage >= 50) { grade = "C"; status = "PASS"; }
        else if (percentage >= 35) { grade = "D"; status = "PASS"; }
        else { grade = "F"; status = "FAIL"; }
    }
};

class Student {
public:
    int id; // SQLite Primary Key
    string rollNo;
    string studentName;
    string motherName;
    string fatherName;
    string collegeName;
    string courseName;
    string regulation;
    Date birthDate;
    vector<Subject> subjects;
    Result result;

    Student() { id = 0; }

    // Convert student to JSON for the API
    json toJson() const {
        json j;
        j["id"] = id;
        j["rollNo"] = rollNo;
        j["studentName"] = studentName;
        j["motherName"] = motherName;
        j["fatherName"] = fatherName;
        j["collegeName"] = collegeName;
        j["courseName"] = courseName;
        j["regulation"] = regulation;
        j["dob"] = birthDate.toString();
        j["result"]["total"] = result.totalMarks;
        j["result"]["percentage"] = result.percentage;
        j["result"]["grade"] = result.grade;
        j["result"]["status"] = result.status;
        
        json subs = json::array();
        for (const auto& sub : subjects) {
            subs.push_back({
                {"name", sub.name},
                {"marks", sub.marks},
                {"max", sub.maxMarks}
            });
        }
        j["subjects"] = subs;
        return j;
    }
};
