#include "student.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int studentManager::idCounter = 0;

student::student(string name, string username, string password)
    : name(name), username(username), password(password), id(-1) {}

student::~student() {}

int student::getId() const { return id; }
string student::getUsername() const { return username; }
string student::getPassword() const { return password; }
string student::getName() const { return name; }
void student::setId(int id) { this->id = id; }

studentManager::studentManager() {
    loadFromFile();
}

bool studentManager::isUsernameUnique(const string& username) const {
    for (int i = 0; i < idCounter; i++) {
        if (studentArray[i].getUsername() == username) {
            return false;
        }
    }
    return true;
}

bool studentManager::isValidPassword(const string& password) const {
    return password.length() > 6;
}

bool studentManager::isValidName(const string& name) const {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool studentManager::registerStudent(const string &fullname, const string &username, const string &password) {
    if (idCounter >= 100) {
        return false;
    }

    if (!isValidName(fullname)) {
        return false;
    }

    do {
        if (!isUsernameUnique(username)) {
            return false;
        }
    } while (!isUsernameUnique(username));

    do {
        if (!isValidPassword(password)) {
            return false;
        }
    } while (!isValidPassword(password));

    studentArray[idCounter] = student(fullname, username, password);
    studentArray[idCounter].setId(idCounter);
    idCounter++;

    saveToFile();
    return true;
}

bool studentManager::login(const string& username, const string& password) {
    for (int i = 0; i < idCounter; i++) {
        if (studentArray[i].getUsername() == username && studentArray[i].getPassword() == password) {
            return true;
        }
    }
    return false;
}

void studentManager::saveToFile() const {
    ofstream outFile("G:\\DUT\\quizz\\students.txt");
    if (!outFile.is_open()) {
        return;
    }

    for (int i = 0; i < idCounter; i++) {
        outFile << studentArray[i].getId() << ","
                << studentArray[i].getName() << ","
                << studentArray[i].getUsername() << ","
                << studentArray[i].getPassword() << "\n";
    }

    outFile.close();
}

void studentManager::loadFromFile() {
    ifstream inFile("G:\\DUT\\quizz\\students.txt");
    if (!inFile.is_open()) {
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, name, username, password;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');

        studentArray[idCounter] = student(name, username, password);
        studentArray[idCounter].setId(idCounter);
        idCounter++;
    }

    inFile.close();
}
