#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// Utility Class for Console Operations
class ConsoleUtils
{
public:
    static void setColor(int foreground, int background)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, foreground + (background * 16));
    }

    static void clearScreen()
    {
        system("cls");
    }

    static void pause()
    {
        getch();
    }
};

// Subject Class for better encapsulation
class Subject
{
private:
    string name;
    float marks;
    float maxMarks;

public:
    Subject() : name(""), marks(0), maxMarks(100) {}
    Subject(string n, float m, float max = 100) : name(n), marks(m), maxMarks(max) {}

    void setMarks(float m) { marks = m; }
    float getMarks() const { return marks; }
    string getName() const { return name; }
    float getMaxMarks() const { return maxMarks; }
};

// Date Class for better date handling
class Date
{
private:
    int day, month, year;

public:
    Date() : day(1), month(1), year(2000) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    void setDate(int d, int m, int y)
    {
        day = d;
        month = m;
        year = y;
    }

    string toString() const
    {
        return (day < 10 ? "0" : "") + to_string(day) + "/" +
               (month < 10 ? "0" : "") + to_string(month) + "/" +
               to_string(year);
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
};

// Result Class to calculate and store results
class Result
{
private:
    float totalMarks;
    float percentage;
    string grade;
    string status;

public:
    Result() : totalMarks(0), percentage(0), grade("-"), status("PENDING") {}

    void calculate(const vector<Subject> &subjects)
    {
        totalMarks = 0;
        for (const auto &subject : subjects)
        {
            totalMarks += subject.getMarks();
        }
        percentage = totalMarks / subjects.size();

        if (percentage >= 80)
        {
            grade = "A+";
            status = "PASS";
        }
        else if (percentage >= 70)
        {
            grade = "A";
            status = "PASS";
        }
        else if (percentage >= 60)
        {
            grade = "B";
            status = "PASS";
        }
        else if (percentage >= 50)
        {
            grade = "C";
            status = "PASS";
        }
        else if (percentage >= 35)
        {
            grade = "D";
            status = "PASS";
        }
        else
        {
            grade = "F";
            status = "FAIL";
        }
    }

    float getTotalMarks() const { return totalMarks; }
    float getPercentage() const { return percentage; }
    string getGrade() const { return grade; }
    string getStatus() const { return status; }
};

// Enhanced Student Class with better encapsulation
class Student
{
private:
    string studentName;
    string motherName;
    string fatherName;
    string collegeName;
    string courseName;
    string rollNo;
    string regulation;
    Date birthDate;
    vector<Subject> subjects;
    Result result;

public:
    Student()
    {
        subjects.push_back(Subject("PHY", 0));
        subjects.push_back(Subject("CHM", 0));
        subjects.push_back(Subject("MAT", 0));
        subjects.push_back(Subject("DSL", 0));
        subjects.push_back(Subject("ECM", 0));
    }

    // Getters
    string getRollNo() const { return rollNo; }
    string getStudentName() const { return studentName; }
    string getMotherName() const { return motherName; }
    string getFatherName() const { return fatherName; }
    string getCollegeName() const { return collegeName; }
    string getCourseName() const { return courseName; }
    string getRegulation() const { return regulation; }
    Date getBirthDate() const { return birthDate; }
    const vector<Subject> &getSubjects() const { return subjects; }
    const Result &getResult() const { return result; }

    // Input method
    void inputDetails()
    {
        ConsoleUtils::clearScreen();
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(4, 14);
        cout << "\t\t\t\t\t\t | Add Student Details |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\t\t\t--> Enter Student Name : ";
        cin.ignore();
        getline(cin, studentName);
        cout << "\t\t\t--> Enter Mother Name : ";
        getline(cin, motherName);
        cout << "\t\t\t--> Enter Father Name : ";
        getline(cin, fatherName);
        cout << "\t\t\t--> Enter College Name : ";
        getline(cin, collegeName);
        cout << "\t\t\t--> Enter Course Name : ";
        getline(cin, courseName);
        cout << "\t\t\t--> Enter Roll Number  : ";
        getline(cin, rollNo);
        cout << "\t\t\t--> Enter Regulation : ";
        getline(cin, regulation);

        int d, m, y;
        cout << "\t\t\t--> Enter Birth Date (DD MM YYYY) : ";
        cin >> d >> m >> y;
        birthDate.setDate(d, m, y);

        ConsoleUtils::setColor(7, 0);
        cout << "\t\t\t-----------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t Enter Your Marks Subject Wise " << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "\t\t\t-----------------------------------------------" << endl;

        for (auto &subject : subjects)
        {
            float marks;
            cout << "\t\t\t--> " << subject.getName() << "  : ";
            cin >> marks;
            subject.setMarks(marks);
        }

        result.calculate(subjects);
    }

    // Display method
    void display() const
    {
        ConsoleUtils::setColor(14, 0);
        cout << "\n\n\t CLASS  : " << courseName << endl;
        cout << "\t COLLEGE  : " << collegeName << endl;

        ConsoleUtils::setColor(6, 0);
        cout << "\n\n\t STUDENT'S NAME : " << studentName;
        cout << "\t\t\t\t\t ROLL NO : " << rollNo << endl;
        cout << "\n\t MOTHER'S NAME : " << motherName;
        cout << "\t\t\t\t\t DATE OF BIRTH : " << birthDate.toString() << endl;
        cout << "\n\t FATHER'S NAME : " << fatherName;
        cout << "\t\t\t\t\t Regulation : " << regulation << endl;

        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(11, 0);
        cout << "\n\t|   Subject Name      |\t  Marks Obtained  |\tOut-Of   |  Percentage   |\tGrade    |";
        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(11, 0);

        for (size_t i = 0; i < subjects.size(); i++)
        {
            cout << "\n\t|\t " << subjects[i].getName() << "          |\t      "
                 << fixed << setprecision(2) << subjects[i].getMarks() << "          |\t"
                 << subjects[i].getMaxMarks() << "\t |";

            if (i == subjects.size() / 2)
            {
                cout << "\t" << result.getPercentage() << "%\t |\t" << result.getGrade() << "\t |";
            }
            else
            {
                cout << "\t\t |" << "\t\t |";
            }
        }

        ConsoleUtils::setColor(2, 0);
        cout << "\n\t+----------------------------------------------------------------------------------------+";
        ConsoleUtils::setColor(9, 0);
        cout << "\n\t\t\t\t Total - " << fixed << setprecision(2) << result.getTotalMarks()
             << "\t\t\t RESULT - " << result.getStatus() << endl;

        cout << "\n\n\t Dated : ";
        cout << "\t\t\t\t\t\t   Principle Signatures : " << endl;
        ConsoleUtils::setColor(2, 0);
        cout << "\n\n\t-----------------------------------------------------------------------------------------------------";
    }

    // File I/O methods
    void saveToFile(ofstream &file) const
    {
        file << studentName << "|" << fatherName << "|" << motherName << "|"
             << collegeName << "|" << courseName << "|" << rollNo << "|"
             << regulation << "|" << birthDate.getDay() << "|" << birthDate.getMonth()
             << "|" << birthDate.getYear();

        for (const auto &subject : subjects)
        {
            file << "|" << subject.getMarks();
        }

        file << "|" << result.getTotalMarks() << "|" << result.getPercentage()
             << "|" << result.getGrade() << "|" << result.getStatus() << "\n";
    }

    bool loadFromFile(ifstream &file)
    {
        string line;
        if (getline(file, line))
        {
            stringstream ss(line);
            string token;
            int field = 0;
            int d, m, y;

            while (getline(ss, token, '|'))
            {
                switch (field)
                {
                case 0:
                    studentName = token;
                    break;
                case 1:
                    fatherName = token;
                    break;
                case 2:
                    motherName = token;
                    break;
                case 3:
                    collegeName = token;
                    break;
                case 4:
                    courseName = token;
                    break;
                case 5:
                    rollNo = token;
                    break;
                case 6:
                    regulation = token;
                    break;
                case 7:
                    d = stoi(token);
                    break;
                case 8:
                    m = stoi(token);
                    break;
                case 9:
                    y = stoi(token);
                    birthDate.setDate(d, m, y);
                    break;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    subjects[field - 10].setMarks(stof(token));
                    break;
                }
                field++;
            }
            result.calculate(subjects);
            return true;
        }
        return false;
    }
};

// Student Management System Class
class StudentManagementSystem
{
private:
    const string filename = "studentresult.txt";
    vector<Student> students;

    void loadAllStudents()
    {
        students.clear();
        ifstream file(filename);
        if (file.is_open())
        {
            Student temp;
            while (temp.loadFromFile(file))
            {
                students.push_back(temp);
            }
            file.close();
        }
    }

    void saveAllStudents()
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (const auto &student : students)
            {
                student.saveToFile(file);
            }
            file.close();
        }
    }

public:
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
            cout << "\t\t\t\t 6. Generate Web Report " << endl;
            cout << "\t\t\t\t 7. Exit " << endl;

            ConsoleUtils::setColor(7, 0);
            cout << "\t\t\t\t-------------------------------------" << endl;
            cout << "\t\t\t\t  Choose Option : [1-7]" << endl;
            cout << "\t\t\t\t-------------------------------------" << endl;
            cout << " --> Please Enter Your Choice : ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                do
                {
                    insertRecord();
                    cout << "\n--> Add New Record (y/n): ";
                    cin >> cont;
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
                generateWebReport();
                break;
            case 7:
                exit(0);
            default:
                cout << "\n-->Invalid Choice..!! Please Try Again " << endl;
            }
            ConsoleUtils::pause();
        } while (true);
    }

    void insertRecord()
    {
        Student student;
        student.inputDetails();

        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            student.saveToFile(file);
            file.close();
            cout << "\n\n\t\t\t Record Added Successfully!" << endl;
        }
        else
        {
            cout << "\n\n\t\t\t Error: Could not open file!" << endl;
        }
    }

    void displayAllRecords()
    {
        ConsoleUtils::clearScreen();
        loadAllStudents();

        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(4, 14);
        cout << "\t\t\t\t\t\t | Student Result Table |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        if (students.empty())
        {
            cout << "\n\t\t\t No Data Is Found ......" << endl;
        }
        else
        {
            for (const auto &student : students)
            {
                student.display();
            }
        }
    }

    void searchRecord()
    {
        ConsoleUtils::clearScreen();
        loadAllStudents();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Search Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\n\t\t\t--> Enter Roll No. : ";
        cin.ignore();
        getline(cin, rollNo);

        bool found = false;
        for (const auto &student : students)
        {
            if (student.getRollNo() == rollNo)
            {
                student.display();
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "\n\t\t\t Student Not Found!" << endl;
        }
    }

    void updateRecord()
    {
        ConsoleUtils::clearScreen();
        loadAllStudents();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Update Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "\n\t\t\t--> Enter Roll No. : ";
        cin.ignore();
        getline(cin, rollNo);

        bool found = false;
        for (auto &student : students)
        {
            if (student.getRollNo() == rollNo)
            {
                cout << "\n\t\t\t Current Record:" << endl;
                student.display();
                cout << "\n\n\t\t\t Enter New Details:" << endl;
                student.inputDetails();
                saveAllStudents();
                cout << "\n\n\t\t\t Record Updated Successfully!" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "\n\t\t\t Student Not Found!" << endl;
        }
    }

    void deleteRecord()
    {
        ConsoleUtils::clearScreen();
        loadAllStudents();

        string rollNo;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Delete Student Record |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        if (students.empty())
        {
            cout << "\n\t\t\t No Data Is Found ......" << endl;
            return;
        }

        cout << "\n\t\t\t--> Enter Roll No. : ";
        cin.ignore();
        getline(cin, rollNo);

        auto it = remove_if(students.begin(), students.end(),
                            [&rollNo](const Student &s)
                            { return s.getRollNo() == rollNo; });

        if (it != students.end())
        {
            students.erase(it, students.end());
            saveAllStudents();
            cout << "\n\t\t\t Record Deleted Successfully!" << endl;
        }
        else
        {
            cout << "\n\t\t\t Student Not Found!" << endl;
        }
    }

    void generateWebReport()
    {
        ConsoleUtils::clearScreen();
        loadAllStudents();

        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        ConsoleUtils::setColor(11, 0);
        cout << "\t\t\t\t\t\t | Generate Web Report |" << endl;
        ConsoleUtils::setColor(7, 0);
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

        ofstream htmlFile("student_report.html");
        if (!htmlFile.is_open())
        {
            cout << "\n\t\t\t Error: Could not create HTML file!" << endl;
            return;
        }

        // Generate HTML content
        htmlFile << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
        htmlFile << "    <meta charset=\"UTF-8\">\n";
        htmlFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
        htmlFile << "    <title>Student Management System - Report</title>\n";
        htmlFile << "    <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n";
        htmlFile << "    <link rel=\"stylesheet\" href=\"styles.css\">\n";
        htmlFile << "</head>\n<body>\n";
        htmlFile << "    <div class=\"container-fluid\">\n";
        htmlFile << "        <header class=\"py-4 mb-4\">\n";
        htmlFile << "            <h1 class=\"text-center display-4 fw-bold text-primary\">Student Management System</h1>\n";
        htmlFile << "            <p class=\"text-center text-muted\">Academic Report - Generated on " << __DATE__ << "</p>\n";
        htmlFile << "        </header>\n\n";

        for (const auto &student : students)
        {
            htmlFile << "        <div class=\"student-card mb-5 p-4 shadow-lg\">\n";
            htmlFile << "            <div class=\"row mb-3\">\n";
            htmlFile << "                <div class=\"col-md-6\">\n";
            htmlFile << "                    <h3 class=\"text-primary\">" << student.getStudentName() << "</h3>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>Roll No:</strong> " << student.getRollNo() << "</p>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>Course:</strong> " << student.getCourseName() << "</p>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>College:</strong> " << student.getCollegeName() << "</p>\n";
            htmlFile << "                </div>\n";
            htmlFile << "                <div class=\"col-md-6 text-md-end\">\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>Father's Name:</strong> " << student.getFatherName() << "</p>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>Mother's Name:</strong> " << student.getMotherName() << "</p>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>DOB:</strong> " << student.getBirthDate().toString() << "</p>\n";
            htmlFile << "                    <p class=\"mb-1\"><strong>Regulation:</strong> " << student.getRegulation() << "</p>\n";
            htmlFile << "                </div>\n";
            htmlFile << "            </div>\n\n";

            htmlFile << "            <table class=\"table table-hover table-bordered\">\n";
            htmlFile << "                <thead class=\"table-dark\">\n";
            htmlFile << "                    <tr>\n";
            htmlFile << "                        <th>Subject</th>\n";
            htmlFile << "                        <th>Marks Obtained</th>\n";
            htmlFile << "                        <th>Max Marks</th>\n";
            htmlFile << "                    </tr>\n";
            htmlFile << "                </thead>\n";
            htmlFile << "                <tbody>\n";

            for (const auto &subject : student.getSubjects())
            {
                htmlFile << "                    <tr>\n";
                htmlFile << "                        <td>" << subject.getName() << "</td>\n";
                htmlFile << "                        <td>" << fixed << setprecision(2) << subject.getMarks() << "</td>\n";
                htmlFile << "                        <td>" << subject.getMaxMarks() << "</td>\n";
                htmlFile << "                    </tr>\n";
            }

            htmlFile << "                </tbody>\n";
            htmlFile << "                <tfoot class=\"table-light\">\n";
            htmlFile << "                    <tr class=\"fw-bold\">\n";
            htmlFile << "                        <td>Total</td>\n";
            htmlFile << "                        <td>" << fixed << setprecision(2) << student.getResult().getTotalMarks() << "</td>\n";
            htmlFile << "                        <td>500</td>\n";
            htmlFile << "                    </tr>\n";
            htmlFile << "                </tfoot>\n";
            htmlFile << "            </table>\n\n";

            htmlFile << "            <div class=\"row mt-3\">\n";
            htmlFile << "                <div class=\"col-md-4\">\n";
            htmlFile << "                    <div class=\"result-box p-3 text-center\">\n";
            htmlFile << "                        <h5>Percentage</h5>\n";
            htmlFile << "                        <p class=\"display-6 text-info\">" << fixed << setprecision(2) << student.getResult().getPercentage() << "%</p>\n";
            htmlFile << "                    </div>\n";
            htmlFile << "                </div>\n";
            htmlFile << "                <div class=\"col-md-4\">\n";
            htmlFile << "                    <div class=\"result-box p-3 text-center\">\n";
            htmlFile << "                        <h5>Grade</h5>\n";
            htmlFile << "                        <p class=\"display-6 text-warning\">" << student.getResult().getGrade() << "</p>\n";
            htmlFile << "                    </div>\n";
            htmlFile << "                </div>\n";
            htmlFile << "                <div class=\"col-md-4\">\n";
            htmlFile << "                    <div class=\"result-box p-3 text-center\">\n";
            htmlFile << "                        <h5>Result</h5>\n";
            htmlFile << "                        <p class=\"display-6 " << (student.getResult().getStatus() == "PASS" ? "text-success" : "text-danger") << "\">" << student.getResult().getStatus() << "</p>\n";
            htmlFile << "                    </div>\n";
            htmlFile << "                </div>\n";
            htmlFile << "            </div>\n";
            htmlFile << "        </div>\n\n";
        }

        htmlFile << "    </div>\n\n";
        htmlFile << "    <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js\"></script>\n";
        htmlFile << "    <script src=\"script.js\"></script>\n";
        htmlFile << "</body>\n</html>";

        htmlFile.close();

        cout << "\n\t\t\t Web Report Generated Successfully!" << endl;
        cout << "\t\t\t File: student_report.html" << endl;
        cout << "\n\t\t\t Opening in default browser..." << endl;

        system("start student_report.html");
    }
};

int main()
{
    StudentManagementSystem sms;
    sms.menu();
    return 0;
}
