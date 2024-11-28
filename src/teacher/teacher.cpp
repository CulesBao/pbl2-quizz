#include "teacher.h"
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

int teacherManager::idCounter = 0;

teacher::teacher(string name, string username, string password)
    : name(name), username(username), password(password) {}

teacher::~teacher() {}

string teacher::getId() const { return id; }
string teacher::getUsername() const { return username; }
string teacher::getPassword() const { return password; }
string teacher::getName() const { return name; }
void teacher::setId(int id) { formattedId(id); }
void teacher::formattedId(int id)
{
    stringstream tmp;
    tmp << "TEA" << setw(3) << setfill('0') << id;
    this->id = tmp.str();
}
bool teacher::setName(string name)
{
    this->name = name;
    return true;
}

bool teacher::setPassword(string password)
{
    this->password = password;
    return true;
}
teacherManager::teacherManager()
{
    if (idCounter != 0)
        idCounter = 0;
    loadFromFile();
}
bool teacherManager::isUsernameUnique(const string &username) const
{
    for (int i = 0; i < idCounter; i++)
    {
        if (teacherArray[i].getUsername() == username)
        {
            return false;
        }
    }
    return true;
}

bool teacherManager::isValidPassword(const string &password) const
{
    return password.length() >= 6;
}

bool teacherManager::isValidName(const string &name) const
{
    for (char c : name)
    {
        if (!isalpha(c) && c != ' ')
        {
            return false;
        }
    }
    if (name.length() < 3)
    {
        return false;
    }
    return true;
}

bool teacherManager::registerTeacher(const string &fullname, const string &username, const string &password)
{
    if (idCounter >= 100)
    {
        return false;
    }

    if (!isValidName(fullname))
    {
        return false;
    }

    do
    {
        if (!isUsernameUnique(username))
        {
            return false;
        }
    } while (!isUsernameUnique(username));

    do
    {
        if (!isValidPassword(password))
        {
            return false;
        }
    } while (!isValidPassword(password));

    teacherArray[idCounter] = teacher(fullname, username, password);
    teacherArray[idCounter].setId(idCounter);
    idCounter++;

    saveToFile();
    return true;
}

bool teacherManager::login(const string &username, const string &password, currentUser &user)
{
    for (int i = 0; i < idCounter; i++)
    {
        if (teacherArray[i].getUsername() == username && teacherArray[i].getPassword() == password)
        {
            user = currentUser(teacherArray[i].getId(), teacherArray[i].getUsername(), teacherArray[i].getPassword(), teacherArray[i].getName(), "teacher");
            return true;
        }
    }
    return false;
}

void teacherManager::saveToFile() const
{
    ofstream outFile("G:\\DUT\\pbl2-quizz\\src\\teacher\\teachers.txt");
    if (!outFile.is_open())
    {
        return;
    }

    for (int i = 0; i < idCounter; i++)
    {
        outFile << teacherArray[i].getId() << ","
                << teacherArray[i].getName() << ","
                << teacherArray[i].getUsername() << ","
                << teacherArray[i].getPassword() << "\n";
    }

    outFile.close();
}

void teacherManager::loadFromFile()
{
    ifstream inFile("G:\\DUT\\pbl2-quizz\\src\\teacher\\teachers.txt");
    if (!inFile.is_open())
    {
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string idStr, name, username, password;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');

        teacherArray[idCounter] = teacher(name, username, password);
        teacherArray[idCounter].setId(idCounter);
        idCounter++;
    }

    inFile.close();
}

bool teacherManager::update(const string id, const string &newPassword, const string &newName)
{
    for (int i = 0; i < this->idCounter; i++)
        if (teacherArray[i].getId() == id)
        {
            teacherArray[i].setName(newName);
            teacherArray[i].setPassword(newPassword);
            saveToFile();
            return true;
        }
    return true;
}
