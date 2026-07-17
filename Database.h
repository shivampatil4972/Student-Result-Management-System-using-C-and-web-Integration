#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "include/sqlite3.h"
#include "Models.h"

using namespace std;

class Database {
private:
    sqlite3* db;

    void executeSQL(const string& sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

public:
    Database() {
        int rc = sqlite3_open("students.db", &db);
        if (rc) {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
            return;
        }
        init();
    }

    ~Database() {
        sqlite3_close(db);
    }

    void init() {
        string createStudentsTable = R"(
            CREATE TABLE IF NOT EXISTS Students (
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                RollNo TEXT UNIQUE NOT NULL,
                StudentName TEXT NOT NULL,
                MotherName TEXT,
                FatherName TEXT,
                CollegeName TEXT,
                CourseName TEXT,
                Regulation TEXT,
                DOB_Day INT,
                DOB_Month INT,
                DOB_Year INT
            );
        )";
        string createSubjectsTable = R"(
            CREATE TABLE IF NOT EXISTS Subjects (
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                StudentID INTEGER,
                SubjectName TEXT,
                Marks REAL,
                MaxMarks REAL,
                FOREIGN KEY(StudentID) REFERENCES Students(ID) ON DELETE CASCADE
            );
        )";
        executeSQL("PRAGMA foreign_keys = ON;");
        executeSQL(createStudentsTable);
        executeSQL(createSubjectsTable);
    }

    bool addStudent(const Student& s) {
        string sql = "INSERT INTO Students (RollNo, StudentName, MotherName, FatherName, CollegeName, CourseName, Regulation, DOB_Day, DOB_Month, DOB_Year) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

        sqlite3_bind_text(stmt, 1, s.rollNo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, s.studentName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, s.motherName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, s.fatherName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, s.collegeName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, s.courseName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, s.regulation.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 8, s.birthDate.day);
        sqlite3_bind_int(stmt, 9, s.birthDate.month);
        sqlite3_bind_int(stmt, 10, s.birthDate.year);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return false;
        }
        int studentId = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);

        string subSql = "INSERT INTO Subjects (StudentID, SubjectName, Marks, MaxMarks) VALUES (?, ?, ?, ?);";
        for (const auto& sub : s.subjects) {
            sqlite3_prepare_v2(db, subSql.c_str(), -1, &stmt, nullptr);
            sqlite3_bind_int(stmt, 1, studentId);
            sqlite3_bind_text(stmt, 2, sub.name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 3, sub.marks);
            sqlite3_bind_double(stmt, 4, sub.maxMarks);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        return true;
    }

    bool deleteStudent(const string& rollNo) {
        string sql = "DELETE FROM Students WHERE RollNo = ?;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;
        sqlite3_bind_text(stmt, 1, rollNo.c_str(), -1, SQLITE_TRANSIENT);
        bool success = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
        return success;
    }

    bool getStudentByRollNo(const string& rollNo, Student& outStudent) {
        string sql = "SELECT ID, RollNo, StudentName, MotherName, FatherName, CollegeName, CourseName, Regulation, DOB_Day, DOB_Month, DOB_Year FROM Students WHERE RollNo = ?;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;
        sqlite3_bind_text(stmt, 1, rollNo.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            outStudent.id = sqlite3_column_int(stmt, 0);
            outStudent.rollNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            outStudent.studentName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            outStudent.motherName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            outStudent.fatherName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            outStudent.collegeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            outStudent.courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            outStudent.regulation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
            outStudent.birthDate = Date(sqlite3_column_int(stmt, 8), sqlite3_column_int(stmt, 9), sqlite3_column_int(stmt, 10));
            sqlite3_finalize(stmt);

            string subSql = "SELECT SubjectName, Marks, MaxMarks FROM Subjects WHERE StudentID = ?;";
            sqlite3_stmt* subStmt;
            if (sqlite3_prepare_v2(db, subSql.c_str(), -1, &subStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(subStmt, 1, outStudent.id);
                while (sqlite3_step(subStmt) == SQLITE_ROW) {
                    Subject sub;
                    sub.name = reinterpret_cast<const char*>(sqlite3_column_text(subStmt, 0));
                    sub.marks = sqlite3_column_double(subStmt, 1);
                    sub.maxMarks = sqlite3_column_double(subStmt, 2);
                    outStudent.subjects.push_back(sub);
                }
                sqlite3_finalize(subStmt);
            }
            outStudent.result.calculate(outStudent.subjects);
            return true;
        }
        sqlite3_finalize(stmt);
        return false;
    }

    bool updateStudent(const Student& s) {
        // First delete, then re-add
        if (deleteStudent(s.rollNo)) {
            return addStudent(s);
        }
        return false;
    }

    vector<Student> getAllStudents() {
        vector<Student> students;
        string sql = "SELECT RollNo FROM Students;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return students;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string rollNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Student s;
            if (getStudentByRollNo(rollNo, s)) {
                students.push_back(s);
            }
        }
        sqlite3_finalize(stmt);
        return students;
    }
};
