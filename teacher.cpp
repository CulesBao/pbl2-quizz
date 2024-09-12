#include "teacher.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int teacherManager::idCounter = 0;

teacher::teacher(string name, string username, string password)
    : name(name), username(username), password(password), id(-1) {}

teacher::~teacher() {}

int teacher::getId() const { return id; }
string teacher::getUsername() const { return username; }
string teacher::getPassword() const { return password; }
string teacher::getName() const { return name; }
void teacher::setId(int id) { this->id = id; }

teacherManager::teacherManager() {
    loadFromFile();
}

bool teacherManager::isUsernameUnique(const string& username) const {
    for (int i = 0; i < idCounter; i++) {
        if (teacherArray[i].getUsername() == username) {
            return false;
        }
    }
    return true;
}

bool teacherManager::isValidPassword(const string& password) const {
    return password.length() > 6;
}

bool teacherManager::isValidName(const string& name) const {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool teacherManager::registerTeacher(const string &fullname, const string &username, const string &password) {
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

    teacherArray[idCounter] = teacher(fullname, username, password);
    teacherArray[idCounter].setId(idCounter);
    idCounter++;

    saveToFile();
    return true;
}

bool teacherManager::login(const string& username, const string& password) {
    for (int i = 0; i < idCounter; i++) {
        if (teacherArray[i].getUsername() == username && teacherArray[i].getPassword() == password) {
            return true;
        }
    }
    return false;
}

void teacherManager::saveToFile() const {
    ofstream outFile("G:\\DUT\\quizz\\teachers.txt");
    if (!outFile.is_open()) {
        return;
    }

    for (int i = 0; i < idCounter; i++) {
        outFile << teacherArray[i].getId() << ","
                << teacherArray[i].getName() << ","
                << teacherArray[i].getUsername() << ","
                << teacherArray[i].getPassword() << "\n";
    }

    outFile.close();
}

void teacherManager::loadFromFile() {
    ifstream inFile("G:\\DUT\\quizz\\teachers.txt");
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

        teacherArray[idCounter] = teacher(name, username, password);
        teacherArray[idCounter].setId(idCounter);
        idCounter++;
    }

    inFile.close();
}
