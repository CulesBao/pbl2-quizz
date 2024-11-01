#include <iostream>  
#include <string>  
using namespace std;

class User {  
private:  
    static User* instance;  
    int id;  
    string username;  
    string password;  
    string fullname;  
    string role;  

    // Constructor 
    User(int id, const string& username, const string& password, const string& fullname, const string& role)   
        : id(id), username(username), password(password), fullname(fullname), role(role) {}  

public:  
    static User* getInstance(int id = 0, const string& username = "", const string& password = "", const string& fullname = "", const string& role = "") {  
        if (instance == nullptr) {  
            instance = new User(id, username, password, fullname, role);  
        }  
        return instance;  
    }

    int getId() const {  
        return id;  
    }  

    string getUsername() const {  
        return username;  
    }  

    string getPassword() const {  
        return password;  
    }  

    string getFullname() const {  
        return fullname;  
    }  

    string getRole() const {  
        return role;  
    }  
};

User* User::instance = nullptr; 

int main() {  
    User* user1 = User::getInstance(1, "user1", "123", "Thắng Ngọt", "student");  

    User* user2 = User::getInstance(2, "user2", "456", "Bảo Ngọt", "teacher"); 

   
    return 0;  
}