#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

/* =========================
   STUDENT CLASS
========================= */
class Student {
public:
    string name;
    string index;

    Student() {}

    Student(string n, string i) {
        name = n;
        index = i;
    }
};

/* =========================
   SESSION CLASS
========================= */
class AttendanceSession {
public:
    string courseCode;
    string date;
    string startTime;
    int duration;

    map<string, string> attendance; // index -> status

    AttendanceSession() {}

    AttendanceSession(string c, string d, string s, int du) {
        courseCode = c;
        date = d;
        startTime = s;
        duration = du;
    }
};

/* =========================
   GLOBAL VARIABLES
========================= */
vector<Student> students;
vector<AttendanceSession> sessions;

/* =========================
   FUNCTION DECLARATIONS
========================= */
void loadStudents();
void saveStudents();

void registerStudent();
void viewStudents();
void searchStudent();

void createSession();
void markAttendance();
void viewReport();

void loadSessions();
void saveSessions();

void mainMenu();

/* =========================
   STUDENT FUNCTIONS
========================= */

void registerStudent() {

    string name, index;

    cin.ignore();

    cout << "Enter Student Name: ";
    getline(cin, name);

    cout << "Enter Index Number: ";
    getline(cin, index);

    students.push_back(Student(name, index));

    cout << "Student registered successfully!\n";
}

void viewStudents() {

    if (students.empty()) {
        cout << "No students found.\n";
        return;
    }

    cout << "\n--- Student List ---\n";

    for (int i = 0; i < students.size(); i++) {
        cout << i + 1 << ". "
             << students[i].name
             << " (" << students[i].index << ")\n";
    }
}

void searchStudent() {

    string index;
    cin.ignore();

    cout << "Enter Index Number: ";
    getline(cin, index);

    for (auto s : students) {

        if (s.index == index) {
            cout << "Found: " << s.name << endl;
            return;
        }
    }

    cout << "Student not found.\n";
}

/* =========================
   SESSION FUNCTIONS
========================= */

void createSession() {

    string code, date, time;
    int duration;

    cin.ignore();

    cout << "Enter Course Code: ";
    getline(cin, code);

    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter Start Time: ";
    getline(cin, time);

    cout << "Enter Duration (hours): ";
    cin >> duration;

    AttendanceSession s(code, date, time, duration);

    sessions.push_back(s);

    cout << "Session created successfully!\n";
}

void markAttendance() {

    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }

    cout << "\n--- Available Sessions ---\n";

    for (int i = 0; i < sessions.size(); i++) {

        cout << i + 1 << ". "
             << sessions[i].courseCode << " - "
             << sessions[i].date << endl;
    }

    int choice;
    cout << "Select session: ";
    cin >> choice;

    if (choice < 1 || choice > sessions.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    AttendanceSession &session = sessions[choice - 1];

    cout << "\nMark Attendance\n";
    cout << "1 = Present\n2 = Absent\n3 = Late\n";

    for (auto s : students) {

        int status;

        cout << "\n" << s.name << " (" << s.index << "): ";
        cin >> status;

        if (status == 1)
            session.attendance[s.index] = "Present";
        else if (status == 2)
            session.attendance[s.index] = "Absent";
        else if (status == 3)
            session.attendance[s.index] = "Late";
        else
            session.attendance[s.index] = "Absent";
    }

    cout << "Attendance marked successfully!\n";
}

/* =========================
   REPORT FUNCTION
========================= */

void viewReport() {

    if (sessions.empty()) {
        cout << "No sessions found.\n";
        return;
    }

    cout << "\n--- Sessions ---\n";

    for (int i = 0; i < sessions.size(); i++) {

        cout << i + 1 << ". "
             << sessions[i].courseCode
             << " (" << sessions[i].date << ")\n";
    }

    int choice;
    cout << "Select session: ";
    cin >> choice;

    if (choice < 1 || choice > sessions.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    AttendanceSession s = sessions[choice - 1];

    int present = 0, absent = 0, late = 0;

    cout << "\n--- Attendance Report ---\n";

    for (auto st : students) {

        string status = s.attendance[st.index];

        cout << st.name << " : " << status << endl;

        if (status == "Present") present++;
        else if (status == "Late") late++;
        else absent++;
    }

    cout << "\nSummary\n";
    cout << "Present: " << present << endl;
    cout << "Late: " << late << endl;
    cout << "Absent: " << absent << endl;
}

/* =========================
   FILE FUNCTIONS
========================= */

void saveStudents() {

    ofstream file("students.txt");

    for (auto s : students) {
        file << s.name << "," << s.index << endl;
    }

    file.close();
}

void loadStudents() {

    ifstream file("students.txt");

    if (!file) return;

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string name, index;

        getline(ss, name, ',');
        getline(ss, index);

        students.push_back(Student(name, index));
    }

    file.close();
}

void saveSessions() {

    ofstream file("sessions.txt");

    for (auto s : sessions) {

        file << s.courseCode << ","
             << s.date << ","
             << s.startTime << ","
             << s.duration << endl;

        for (auto a : s.attendance) {

            file << a.first << ","
                 << a.second << endl;
        }

        file << "END\n";
    }

    file.close();
}

void loadSessions() {

    ifstream file("sessions.txt");

    if (!file) return;

    string line;

    while (getline(file, line)) {

        if (line.empty()) continue;

        stringstream ss(line);

        AttendanceSession s;

        getline(ss, s.courseCode, ',');
        getline(ss, s.date, ',');
        getline(ss, s.startTime, ',');
        ss >> s.duration;

        while (getline(file, line)) {

            if (line == "END") break;

            stringstream sa(line);

            string index, status;

            getline(sa, index, ',');
            getline(sa, status);

            s.attendance[index] = status;
        }

        sessions.push_back(s);
    }

    file.close();
}

/* =========================
   MAIN MENU
========================= */

void mainMenu() {

    int choice;

    do {

        cout << "\n====== DIGITAL ATTENDANCE SYSTEM ======\n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Create Session\n";
        cout << "5. Mark Attendance\n";
        cout << "6. View Report\n";
        cout << "7. Save Data\n";
        cout << "8. Exit\n";
        cout << "Choose: ";

        cin >> choice;

        switch (choice) {

        case 1: registerStudent(); break;
        case 2: viewStudents(); break;
        case 3: searchStudent(); break;
        case 4: createSession(); break;
        case 5: markAttendance(); break;
        case 6: viewReport(); break;
        case 7:
            saveStudents();
            saveSessions();
            cout << "Data saved!\n";
            break;

        case 8:
            saveStudents();
            saveSessions();
            cout << "Goodbye!\n";
            break;

        default:
            cout << "Invalid option.\n";
        }

    } while (choice != 8);
}

/* =========================
   MAIN FUNCTION
========================= */

int main() {

    loadStudents();
    loadSessions();

    mainMenu();

    return 0;
}