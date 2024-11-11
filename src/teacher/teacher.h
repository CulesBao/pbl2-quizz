#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include "../currentUser/currentUser.h"
using namespace std;

class teacher
{
private:
    string id;
    string name;
    string username;
    string password;

public:
    teacher(string name = "", string username = "", string password = "");
    ~teacher();

    string getId() const;
    string getUsername() const;
    string getPassword() const;
    string getName() const;
    bool setName(string name);
    bool setPassword(string password);
    void setId(int id);
    void formattedId(int id);
};

class teacherManager
{
private:
    teacher teacherArray[1000];
    static int idCounter;

    bool isValidName(const string &name) const;
    bool isUsernameUnique(const string &username) const;
    bool isValidPassword(const string &password) const;

    void saveToFile() const;
    void loadFromFile();

public:
    teacherManager();
    bool registerTeacher(const string &fullname, const string &username, const string &password);
    bool login(const string &username, const string &password, currentUser &user);
    bool update(const string id, const string &newPassword, const string &newName);
};

#endif // TEACHER_H
