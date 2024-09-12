#ifndef TEACHER_H
#define TEACHER_H

#include <string>
using namespace std;

class teacher {
private:
    int id;
    std::string name;
    std::string username;
    std::string password;

public:
    teacher(std::string name = "", std::string username = "", std::string password = "");
    ~teacher();

    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getName() const;
    void setId(int id);
};

class teacherManager {
private:
    teacher teacherArray[100];
    static int idCounter;

    bool isValidName(const std::string& name) const;
    bool isUsernameUnique(const std::string& username) const;
    bool isValidPassword(const std::string& password) const;

    void saveToFile() const;
    void loadFromFile();

public:
    teacherManager();
    bool registerTeacher(const string &fullname, const string &username, const string &password);
    bool login(const std::string& username, const std::string& password);
};

#endif // TEACHER_H
