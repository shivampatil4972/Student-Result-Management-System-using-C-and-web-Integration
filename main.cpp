#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif

// New Modules for Full-Stack Integration
#include "Models.h"
#include "Database.h"
#include "Server.h"

using namespace std;

// Utility Class for Console Operations
class ConsoleUtils
{
public:
    static void setColor(int foreground, int background)
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, foreground + (background * 16));
#endif
    }

    static void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    static void pause()
    {
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    static float getSafeNumericInput(float minVal = -999999, float maxVal = 999999)
    {
        float value;
        while (true)
        {
            if (cin >> value && value >= minVal && value <= maxVal)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            if (cin.eof()) exit(0);
            cout << "\t\t\t--> Invalid input. Please enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    static string getSafeStringInput()
    {
        string input;
        getline(cin, input);
        // Remove pipe characters to prevent delimiter collision (legacy, but good to keep)
        input.erase(remove(input.begin(), input.end(), '|'), input.end());
        return input;
    }
};

// Student Class Extender for Console Input/Display
class ConsoleStudent : public Student {
public:
    void inputDetails()
    {
        ConsoleUtils::clearScreen();
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(4, 14);
        cout << "\t\t\t\t\t\t | Add Student Details |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\t\t\t--> Enter Student Name : ";
        studentName = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter Mother Name : ";
        motherName = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter Father Name : ";
        fatherName = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter College Name : ";
        collegeName = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter Course Name : ";
        courseName = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter Roll Number  : ";
        rollNo = ConsoleUtils::getSafeStringInput();
        cout << "\t\t\t--> Enter Regulation : ";
        regulation = ConsoleUtils::getSafeStringInput();

        int d, m, y;
        cout << "\t\t\t--> Enter Birth Date Day (1-31) : ";
        d = ConsoleUtils::getSafeNumericInput(1, 31);
        cout << "\t\t\t--> Enter Birth Date Month (1-12) : ";
        m = ConsoleUtils::getSafeNumericInput(1, 12);
        cout << "\t\t\t--> Enter Birth Date Year : ";
        y = ConsoleUtils::getSafeNumericInput(1900, 2100);
        birthDate = Date(d, m, y);

        // Pre-fill subjects if empty
        if (subjects.empty()) {
            subjects.push_back(Subject("PHY", 0));
            subjects.push_back(Subject("CHM", 0));
            subjects.push_back(Subject("MAT", 0));
            subjects.push_back(Subject("DSL", 0));
            subjects.push_back(Subject("ECM", 0));
        }

        ConsoleUtils::setColor(7, 0);
        cout << "\t\t\t-----------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t Enter Your Marks Subject Wise " << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "\t\t\t-----------------------------------------------" << endl;

        for (auto &subject : subjects)
        {
            float marks;
            cout << "\t\t\t--> " << subject.name << "  : ";
            marks = ConsoleUtils::getSafeNumericInput(0, subject.maxMarks);
            subject.marks = marks;
        }

        result.calculate(subjects);
    }

    void display() const
    {
        ConsoleUtils::setColor(14, 0);
        cout << "\n\n\t CLASS  : " << courseName << endl;
        cout << "\t COLLEGE  : " << collegeName << endl;

        ConsoleUtils::setColor(6, 0);
        cout << "\n\n\t " << left << setw(40) << ("STUDENT'S NAME : " + studentName) 
             << "ROLL NO : " << rollNo << endl;
        cout << "\n\t " << left << setw(40) << ("MOTHER'S NAME  : " + motherName)
             << "DATE OF BIRTH : " << birthDate.toString() << endl;
        cout << "\n\t " << left << setw(40) << ("FATHER'S NAME  : " + fatherName)
             << "REGULATION    : " << regulation << endl;

        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(11, 0);
        cout << "\n\t|   Subject Name      |\t  Marks Obtained  |\tOut-Of   |  Percentage   |\tGrade    |";
        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(11, 0);

        for (size_t i = 0; i < subjects.size(); i++)
        {
            cout << "\n\t|\t " << subjects[i].name << "          |\t      "
                 << fixed << setprecision(2) << subjects[i].marks << "          |\t"
                 << subjects[i].maxMarks << "\t |";

            if (i == subjects.size() / 2)
            {
                cout << "\t" << result.percentage << "%\t |\t" << result.grade << "\t |";
            }
            else
            {
                cout << "\t\t |" << "\t\t |";
            }
        }

        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(9, 0);
        cout << "\n\t\t\t\t Total - " << fixed << setprecision(2) << result.totalMarks
             << "\t\t\t RESULT - " << result.status << endl;

        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char dateBuf[20];
        strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", now);

        cout << "\n\n\t Dated : " << dateBuf;
        cout << "\t\t\t\t\t   Principle Signatures : " << endl;
        ConsoleUtils::setColor(2, 0);
        cout << "\n\n\t-----------------------------------------------------------------------------------------------------";
    }
};

// Student Management System Class
class StudentManagementSystem
{
private:
    Database db;
    WebServer server;

public:
    StudentManagementSystem() : server(db) {}

    void menu()
    {
        int choice;
        char cont;

        do
        {
            ConsoleUtils::clearScreen();
            cout << "\n\n\t\t\t\t-------------------------------------" << endl;
            ConsoleUtils::setColor(4, 14);
            cout << "\t\t\t\t |    STUDENT MANAGEMENT SYSTEM    | " << endl;
            ConsoleUtils::setColor(7, 0);
            cout << "\t\t\t\t-------------------------------------" << endl;
            ConsoleUtils::setColor(11, 0);
            cout << "\t\t\t\t 1. Enter New Record " << endl;
            cout << "\t\t\t\t 2. Display All Records " << endl;
            cout << "\t\t\t\t 3. Search Record " << endl;
            cout << "\t\t\t\t 4. Update Record " << endl;
            cout << "\t\t\t\t 5. Delete Record " << endl;
            cout << "\t\t\t\t 6. Start Live Web Server (Dashboard) " << endl;
            cout << "\t\t\t\t 7. Stop Live Web Server " << endl;
            cout << "\t\t\t\t 8. Exit " << endl;

            ConsoleUtils::setColor(7, 0);
            cout << "\t\t\t\t-------------------------------------" << endl;
            cout << "\t\t\t\t  Choose Option : [1-8]" << endl;
            cout << "\t\t\t\t-------------------------------------" << endl;
            cout << " --> Please Enter Your Choice : ";
            choice = ConsoleUtils::getSafeNumericInput(1, 8);

            switch (choice)
            {
            case 1:
                do
                {
                    insertRecord();
                    cout << "\n--> Add Another Record (y/n): ";
                    string c = ConsoleUtils::getSafeStringInput();
                    cont = c.empty() ? 'n' : c[0];
                } while (cont == 'Y' || cont == 'y');
                break;
            case 2:
                displayAllRecords();
                break;
            case 3:
                searchRecord();
                break;
            case 4:
                updateRecord();
                break;
            case 5:
                deleteRecord();
                break;
            case 6:
                server.start();
                break;
            case 7:
                server.stop();
                break;
            case 8:
                server.stop();
                exit(0);
            default:
                cout << "\n-->Invalid Choice..!! Please Try Again " << endl;
            }
            ConsoleUtils::pause();
        } while (true);
    }

    void insertRecord()
    {
        ConsoleStudent student;
        student.inputDetails();

        if (db.addStudent(student)) {
            cout << "\n\n\t\t\t Record Added Successfully to SQLite Database!" << endl;
        } else {
            cout << "\n\n\t\t\t Failed! Roll No may already exist." << endl;
        }
    }

    void displayAllRecords()
    {
        ConsoleUtils::clearScreen();
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(4, 14);
        cout << "\t\t\t\t\t\t | Student Result Table |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        vector<Student> students = db.getAllStudents();
        if (students.empty())
        {
            cout << "\n\t\t\t No Data Is Found in Database..." << endl;
        }
        else
        {
            for (const auto &s : students)
            {
                // Cast to ConsoleStudent for display method
                ConsoleStudent cs;
                cs.rollNo = s.rollNo;
                cs.studentName = s.studentName;
                cs.motherName = s.motherName;
                cs.fatherName = s.fatherName;
                cs.collegeName = s.collegeName;
                cs.courseName = s.courseName;
                cs.regulation = s.regulation;
                cs.birthDate = s.birthDate;
                cs.subjects = s.subjects;
                cs.result = s.result;
                cs.display();
            }
        }
    }

    void searchRecord()
    {
        ConsoleUtils::clearScreen();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Search Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\n\t\t\t--> Enter Roll No. : ";
        rollNo = ConsoleUtils::getSafeStringInput();

        Student s;
        if (db.getStudentByRollNo(rollNo, s))
        {
            ConsoleStudent cs;
            cs.rollNo = s.rollNo; cs.studentName = s.studentName; cs.motherName = s.motherName;
            cs.fatherName = s.fatherName; cs.collegeName = s.collegeName; cs.courseName = s.courseName;
            cs.regulation = s.regulation; cs.birthDate = s.birthDate; cs.subjects = s.subjects; cs.result = s.result;
            cs.display();
        }
        else
        {
            cout << "\n\t\t\t Student Not Found in Database!" << endl;
        }
    }

    void updateRecord()
    {
        ConsoleUtils::clearScreen();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Update Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\n\t\t\t--> Enter Roll No. : ";
        rollNo = ConsoleUtils::getSafeStringInput();

        Student s;
        if (db.getStudentByRollNo(rollNo, s))
        {
            cout << "\n\t\t\t Current Record:" << endl;
            ConsoleStudent cs;
            cs.rollNo = s.rollNo; cs.studentName = s.studentName; cs.motherName = s.motherName;
            cs.fatherName = s.fatherName; cs.collegeName = s.collegeName; cs.courseName = s.courseName;
            cs.regulation = s.regulation; cs.birthDate = s.birthDate; cs.subjects = s.subjects; cs.result = s.result;
            cs.display();

            cout << "\n\n\t\t\t Enter New Details:" << endl;
            ConsoleStudent newStudent;
            newStudent.inputDetails();
            // Retain original rollNo if they didn't want to change it? The prompt asks for all.
            // If we update, we delete old and insert new.
            
            if (db.deleteStudent(rollNo) && db.addStudent(newStudent)) {
                cout << "\n\n\t\t\t Record Updated Successfully in Database!" << endl;
            } else {
                cout << "\n\n\t\t\t Failed to update!" << endl;
            }
        }
        else
        {
            cout << "\n\t\t\t Student Not Found!" << endl;
        }
    }

    void deleteRecord()
    {
        ConsoleUtils::clearScreen();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Delete Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\n\t\t\t--> Enter Roll No. : ";
        rollNo = ConsoleUtils::getSafeStringInput();

        if (db.deleteStudent(rollNo))
        {
            cout << "\n\t\t\t Record Deleted Successfully from Database!" << endl;
        }
        else
        {
            cout << "\n\t\t\t Student Not Found!" << endl;
        }
    }
};

int main()
{
    StudentManagementSystem sms;
    sms.menu();
    return 0;
}
