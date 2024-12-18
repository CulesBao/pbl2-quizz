#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include <string>
using namespace std;

class currentUser
{
private:
    string id;
    string username;
    string password;
    string fullname;
    string role;

public:
    currentUser();
    currentUser(string id, const string &username, const string &password, const string &fullname, const string &role);
    string getId() const;
    string getUsername() const;
    string getPassword() const;
    string getFullname() const;
    string getRole() const;
    void setFullname(const string &fullname);
    void setPassword(const string &password);
    void loadFromFile();
    void saveToFile();
    ~currentUser();
};

#endif