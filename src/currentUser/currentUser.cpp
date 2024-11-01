#include "currentUser.h"
#include <bits/stdc++.h>

using namespace std;

currentUser::currentUser(int id, const string& username, const string& password, const string& fullname, const string& role)
    : id(id), username(username), password(password), fullname(fullname), role(role) {}

int currentUser::getId() const {
    return id;
}

string currentUser::getUsername() const {
    return username;
}

string currentUser::getPassword() const {
    return password;
}

string currentUser::getFullname() const {
    return fullname;
}

string currentUser::getRole() const {
    return role;
}
