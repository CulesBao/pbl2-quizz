#include "currentUser.h"
#include <bits/stdc++.h>

using namespace std;

currentUser::currentUser()
{
}
currentUser::currentUser(string id, const string &username, const string &password, const string &fullname, const string &role)
    : id(id), username(username), password(password), fullname(fullname), role(role)
{
    saveToFile();
}

string currentUser::getId() const
{
    return id;
}

string currentUser::getUsername() const
{
    return username;
}

string currentUser::getPassword() const
{
    return password;
}

string currentUser::getFullname() const
{
    return fullname;
}

string currentUser::getRole() const
{
    return role;
}
void currentUser::loadFromFile()
{
    ifstream file("G:\\DUT\\pbl2-quizz\\src\\currentUser\\currentUser.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            id = line;
            getline(file, line);
            username = line;
            getline(file, line);
            password = line;
            getline(file, line);
            fullname = line;
            getline(file, line);
            role = line;
        }
        file.close();
    }
}
void currentUser::saveToFile()
{
    ofstream file("G:\\DUT\\pbl2-quizz\\src\\currentUser\\currentUser.txt");
    if (file.is_open())
    {
        file << id << endl;
        file << username << endl;
        file << password << endl;
        file << fullname << endl;
        file << role << endl;
        file.close();
    }
}

currentUser::~currentUser()
{
}
