#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include <string>
using namespace std;

class currentUser {
private:
    string id;
    string username;
    string password;
    string fullname;
    string role;

    // Constructor 

public:
    currentUser(string id, const string& username, const string& password, const string& fullname, const string& role);
    string getId() const;
    string getUsername() const;
    string getPassword() const;
    string getFullname() const;
    string getRole() const;
};

#endif // CURRENTUSER_H
