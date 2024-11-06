#include <bits/stdc++.h>
#include "testInfo.h"

using namespace std;

// Constructor
Test::Test()
    : id(0), teacherId(0), title(""), totalQuestion(0), password(""),
      duration(0), startsAt(""), endsAt(""), status(INCOMING)
{
}

Test::Test(int teacherId, string title, int totalQuestion, string password,
           int duration, string startsAt, string endsAt, Status status)
    : teacherId(teacherId), title(title), totalQuestion(totalQuestion), password(password),
      duration(duration), startsAt(startsAt), endsAt(endsAt), status(status)
{
    id = ++currentId;
}

// Cac ham GET
Test::~Test() {}
int Test::getId() const { return id; }
int Test::getTeacherId() const { return teacherId; }
string Test::getTitle() const { return title; }
int Test::getTotalQuestion() const { return totalQuestion; }
string Test::getPassword() const { return password; }
int Test::getDuration() const { return duration; }
string Test::getStartsAt() const { return startsAt; }
string Test::getEndsAt() const { return endsAt; }
Test::Status Test::getStatus() const { return status; }

// Cac ham SET
void Test::setId(int id) { this->id = id; }
void Test::setTeacherId(int teacherId) { this->teacherId = teacherId; }
void Test::setTitle(string title) { this->title = title; }
void Test::setTotalQuestion(int totalQuestion) { this->totalQuestion = totalQuestion; }
void Test::setPassword(string password) { this->password = password; }
void Test::setDuration(int duration) { this->duration = duration; }
void Test::setStartsAt(string startsAt) { this->startsAt = startsAt; }
void Test::setEndsAt(string endsAt) { this->endsAt = endsAt; }
void Test::setStatus(Status newStatus) { this->status = newStatus; }

int Test::currentId = 0;

TestManager::TestManager()
{
    loadFromFile();
}

// HAM LUU VAO TEP
void TestManager::saveToFile() const
{
    ofstream outFile("G:\\DUT\\pbl2-quizz\\src\\testInfo\\testInfo.txt");
    if (!outFile.is_open())
    {
        return;
    }

    for (int i = 0; i < testCount; i++)
    {
        outFile << managerTest[i].getId() << ","
                << managerTest[i].getTeacherId() << ","
                << managerTest[i].getTitle() << ","
                << managerTest[i].getTotalQuestion() << ","
                << managerTest[i].getPassword() << ","
                << managerTest[i].getDuration() << ","
                << managerTest[i].getStartsAt() << ","
                << managerTest[i].getEndsAt() << ","
                << managerTest[i].getStatus() << "\n";
    }
    outFile.close();
}

// HAM DOC TEP
void TestManager::loadFromFile()
{
    ifstream inFile("G:\\DUT\\pbl2-quizz\\src\\testInfo\\testInfo.txt");
    if (!inFile.is_open())
    {
        return;
    }
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string idStr, teacherIdStr, title, totalQuestionStr, password, durationStr, startsAt, endsAt, statusStr;

        if (!getline(ss, idStr, ',') ||
            !getline(ss, teacherIdStr, ',') ||
            !getline(ss, title, ',') ||
            !getline(ss, totalQuestionStr, ',') ||
            !getline(ss, password, ',') ||
            !getline(ss, durationStr, ',') ||
            !getline(ss, startsAt, ',') ||
            !getline(ss, endsAt, ',') ||
            !getline(ss, statusStr))
        {
            continue;
        }

        int teacherId = stoi(teacherIdStr);
        int duration = stoi(durationStr);
        int totalQuestion = stoi(totalQuestionStr);
        Test::Status status = static_cast<Test::Status>(stoi(statusStr));

        managerTest[testCount] = Test(teacherId, title, totalQuestion, password, duration, startsAt, endsAt, status);
        managerTest[testCount].setId(stoi(idStr));
        testCount++;
    }
    inFile.close();
}

// KTRA MAT KHAU
bool TestManager::validatePassword(const string &password) const
{
    if (password.length() <= 5)
        return false;
    return true;
}

// KTRA TEACHERID
bool TestManager::validateTeacherId(const int &teacherId) const
{
    return teacherId >= 0;
}

// KTRA THOI GIAN LAM BAI
bool TestManager::validateDuration(const int &duration) const
{
    return duration >= 10;
}

// KTRA TIEU DE BAI THI
bool TestManager::validateTitle(const string &title) const
{
    if (title.empty())
    {
        return false;
    }
    return true;
}

// KTRA SO LUONG CAU HOI
bool TestManager::validateTotalQuestion(const int &totalQuestion) const
{
    return totalQuestion > 4;
}

// KTRA THOI GIAN BAT DAU
bool TestManager::validateStartsAt(const std::string &startsAt) const
{

    if (startsAt.length() != 19)
    {
        return false;
    }

    if (startsAt[2] != ':' || startsAt[5] != ':' || startsAt[8] != ' ' || startsAt[11] != '/' || startsAt[14] != '/')
    {
        return false;
    }
    int hour = stoi(startsAt.substr(0, 2));
    int minute = stoi(startsAt.substr(3, 2));
    int second = stoi(startsAt.substr(6, 2));
    int day = stoi(startsAt.substr(9, 2));
    int month = stoi(startsAt.substr(12, 2));
    int year = stoi(startsAt.substr(15, 4));

    if (hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59 ||
        day < 1 || day > 31 ||
        month < 1 || month > 12)
    {
        return false;
    }
    if ((month == 2 && day > 29) ||
        ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30))
    {
        return false;
    }
    if (month == 2 && day == 29)
    {
        if (!((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        {
            return false;
        }
    }
    return true;
}

// KTRA THOI GIAN KET THUC
bool TestManager::validateEndsAt(const std::string &endsAt) const
{

    if (endsAt.length() != 19)
    {
        return false;
    }

    if (endsAt[2] != ':' || endsAt[5] != ':' || endsAt[8] != ' ' || endsAt[11] != '/' || endsAt[14] != '/')
    {
        return false;
    }

    int hour = stoi(endsAt.substr(0, 2));
    int minute = stoi(endsAt.substr(3, 2));
    int second = stoi(endsAt.substr(6, 2));
    int day = stoi(endsAt.substr(9, 2));
    int month = stoi(endsAt.substr(12, 2));
    int year = stoi(endsAt.substr(15, 4));

    if (hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 ||
        second < 0 || second > 59 ||
        day < 1 || day > 31 ||
        month < 1 || month > 12)
    {
        return false;
    }
    if ((month == 2 && day > 29) ||
        ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30))
    {
        return false;
    }
    if (month == 2 && day == 29)
    {
        if (!((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        {
            return false;
        }
    }
    return true;
}

// TAO BAI THI
bool TestManager::createTest(const int &teacherId, const string &title, const int &totalQuestion,
                             const string &password, const int &duration, const string &startsAt, const string &endsAt)
{
    if (testCount >= 1000)
    {
        return false;
    }
    if (!validateTeacherId(teacherId))
    {
        return false;
    }
    if (!validateTitle(title))
    {
        return false;
    }
    if (!validateTotalQuestion(totalQuestion))
    {
        return false;
    }
    if (!validatePassword(password))
    {
        return false;
    }
    if (!validateDuration(duration))
    {
        return false;
    }
    if (!validateStartsAt(startsAt))
    {
        return false;
    }
    if (!validateEndsAt(endsAt))
    {
        return false;
    }
    Test newTest(teacherId, title, totalQuestion, password, duration, startsAt, endsAt);
    addTest(newTest);
    saveToFile();
    return true;
}

// CAP NHAT BAI THI
bool TestManager::updateTest(const int &testId, const int &teacherId, const string &title, const int &totalQuestion,
                             const string &password, const int &duration, const string &startsAt, const string &endsAt)
{

    Test *testToUpdate = nullptr;
    for (int i = 0; i < testCount; ++i)
    {
        if (managerTest[i].getId() == testId)
        {
            testToUpdate = &managerTest[i];
            break;
        }
    }

    if (!testToUpdate)
    {
        return false;
    }

    if (!validateTeacherId(teacherId))
    {
        return false;
    }
    if (!validateTitle(title))
    {
        return false;
    }
    if (!validateTotalQuestion(totalQuestion))
    {
        return false;
    }
    if (!validatePassword(password))
    {
        return false;
    }
    if (!validateDuration(duration))
    {
        return false;
    }
    if (!validateStartsAt(startsAt))
    {
        return false;
    }
    if (!validateEndsAt(endsAt))
    {
        return false;
    }

    testToUpdate->setTeacherId(teacherId);
    testToUpdate->setTitle(title);
    testToUpdate->setTotalQuestion(totalQuestion);
    testToUpdate->setPassword(password);
    testToUpdate->setDuration(duration);
    testToUpdate->setStartsAt(startsAt);
    testToUpdate->setEndsAt(endsAt);

    saveToFile();
    return true;
}

bool TestManager::checkPassword(const Test &test, const string &pass) const
{
    return test.getPassword() == pass;
}

// THEM BAI KIEM TRA
bool TestManager::addTest(const Test &newTest)
{
    if (testCount < 1000)
    {
        managerTest[testCount] = newTest;
        testCount++;
        return true;
    }
    return false;
}

// TIM KIEM BAI KTRA
Test *TestManager::getTestById(const int testId)
{
    for (int i = 0; i < testCount; ++i)
    {
        if (managerTest[i].getId() == testId)
        {
            managerTest[i].updateStatus();
            return &managerTest[i];
        }
    }
    return nullptr;
}

// CAP NHAT TRANG THAI
void Test::updateStatus()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *currentLocalTime = localtime(&currentTime);

    auto parseDateTime = [](const string &dateTimeStr) -> tm
    {
        tm timeStruct = {};

        int hour, minute, second, day, month, year;
        sscanf(dateTimeStr.c_str(), "%d:%d:%d %d/%d/%d", &hour, &minute, &second, &day, &month, &year);

        timeStruct.tm_hour = hour;
        timeStruct.tm_min = minute;
        timeStruct.tm_sec = second;
        timeStruct.tm_mday = day;
        timeStruct.tm_mon = month - 1;
        timeStruct.tm_year = year - 1900;

        return timeStruct;
    };

    tm start = parseDateTime(startsAt);
    tm end = parseDateTime(endsAt);

    time_t start_time = mktime(&start);
    time_t end_time = mktime(&end);

    if (currentTime < start_time)
    {
        status = INCOMING;
    }
    else if (currentTime >= start_time && currentTime <= end_time)
    {
        status = RUNNING;
    }
    else
    {
        status = COMPLETE;
    }
}

// DEM TEST CHUA LAM
int TestManager::countIncomingTest(int teacherId)
{
    int count = 0;
    for (int i = 0; i < testCount; ++i)
    {
        managerTest[i].updateStatus();
        if (managerTest[i].getStatus() == 0 && managerTest[i].getTeacherId() == teacherId)
        {
            ++count;
        }
    }
    return count;
}

// DEM TEST DANG LAM
int TestManager::countRunningTest(int teacherId)
{
    int count = 0;
    for (int i = 0; i < testCount; ++i)
    {
        managerTest[i].updateStatus();
        if (managerTest[i].getStatus() == 1 && managerTest[i].getTeacherId() == teacherId)
        {
            ++count;
        }
    }
    return count;
}
// DEM TEST DA LAM
int TestManager::countCompletedTest(int teacherId)
{
    int count = 0;
    for (int i = 0; i < testCount; ++i)
    {
        managerTest[i].updateStatus();
        if (managerTest[i].getStatus() == 2 && managerTest[i].getTeacherId() == teacherId)
        {
            ++count;
        }
    }
    return count;
}
// SO LUONG BAI KIEM TRA
int TestManager::getTestCount() const
{
    return testCount;
}
// TIM BAI KIEM TRA THEO TEACHERID
Test *TestManager::getTestByTeacherId(const int &teacherId, int &count)
{
    Test *teacherTests = new Test[1000];
    count = 0;
    for (int i = 0; i < testCount; ++i)
    {
        if (managerTest[i].getTeacherId() == teacherId)
        {
            teacherTests[count] = managerTest[i];
            ++count;
        }
    }
    return teacherTests;
}