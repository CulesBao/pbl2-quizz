#ifndef STUDENT_H
#define STUDENT_H
#include <bits/stdc++.h>
#include "../currentUser/currentUser.h"

using namespace std;

class student
{
private:
    string id;
    string name;
    string username;
    string password;

public:
    student(string name = "", string username = "", string password = "");
    ~student();

    string getId() const;
    string getUsername() const;
    string getPassword() const;
    string getName() const;
    bool setName(const string &name);
    bool setPassword(const string &password);
    void setId(int id);
    void formattedId(int id);
};

class studentManager
{
private:
    student studentArray[1000];
    static int idCounter;

    bool isValidName(const string &name) const;
    bool isUsernameUnique(const string &username) const;
    bool isValidPassword(const string &password) const;

    void saveToFile() const;
    void loadFromFile();

public:
    studentManager();
    int getIdCounter() const;
    bool registerStudent(const string &fullname, const string &username, const string &password);
    bool login(const string &username, const string &password, currentUser &user);
    bool update(const string id, const string &newPassword, const string &newName);
    string getNameById(const string id);
};
#endif // STUDENT_H
