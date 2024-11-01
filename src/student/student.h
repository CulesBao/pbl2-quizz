#ifndef STUDENT_H
#define STUDENT_H
    #include <bits/stdc++.h>
    #include "../currentUser/currentUser.h"

    using namespace std;

    class student {
    private:
        int id;
        string name;
        string username;
        string password;

    public:
        student(string name = "", string username = "", string password = "");
        ~student();

        int getId() const;
        string getUsername() const;
        string getPassword() const;
        string getName() const;
        void setId(int id);
    };

    class studentManager {
    private:
        student studentArray[100];
        static int idCounter;

        bool isValidName(const string& name) const;
        bool isUsernameUnique(const string& username) const;
        bool isValidPassword(const string& password) const;

        void saveToFile() const;
        void loadFromFile();

    public:
        studentManager();
        bool registerStudent(const string &fullname, const string &username, const string &password);
        bool login(const string& username, const string& password, currentUser& user);
    };
#endif // STUDENT_H
