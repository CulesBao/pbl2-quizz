#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include <string>
using namespace std;

class currentUser {
private:
    int id;
    string username;
    string password;
    string fullname;
    string role;

    // Constructor 

public:
    currentUser(int id, const string& username, const string& password, const string& fullname, const string& role);
    int getId() const;
    string getUsername() const;
    string getPassword() const;
    string getFullname() const;
    string getRole() const;
};

#endif // CURRENTUSER_H
