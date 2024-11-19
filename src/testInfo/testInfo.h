#ifndef TEST_H
#define TEST_H

#include <string>
using namespace std;

class Test
{
public:
    enum Status
    {
        INCOMING,
        RUNNING,
        COMPLETE
    };

private:
    static int currentId;
    string id;
    string teacherId;
    string title;
    int totalQuestion;
    string password;
    int duration;
    string startsAt;
    string endsAt;
    Status status;

public:
    Test();
    Test(string teacherId, string title, int totalQuestion, string password,
         int duration, string startsAt, string endsAt, Status status = INCOMING);
    ~Test();

    string getId() const;
    string getTeacherId() const;
    string getTitle() const;
    int getTotalQuestion() const;
    string getPassword() const;
    int getDuration() const;
    string getStartsAt() const;
    string getEndsAt() const;
    Status getStatus() const;

    void setId(string id);
    void setTeacherId(string teacherId);
    void setTitle(string title);
    void setTotalQuestion(int totalQuestion);
    void setPassword(string password);
    void setDuration(int duration);
    void setStartsAt(string startsAt);
    void setEndsAt(string endsAt);
    void setStatus(Status status);

    void updateStatus();
};

class TestManager
{
private:
    Test managerTest[100];
    int testCount = 0;

    bool validateTeacherId(const std::string &teacherId) const;
    bool validatePassword(const std::string &password) const;
    bool validateDuration(const int &duration) const;
    bool validateTitle(const std::string &title) const;
    bool validateTotalQuestion(const int &totalQuestion) const;
    bool validateStartsAt(const std::string &startsAt) const;
    bool validateEndsAt(const std::string &endsAt) const;

public:
    TestManager();
    void saveToFile() const;
    void loadFromFile();
    bool addTest(const Test &newTest);
    int getTestCount() const;

    Test *getTestById(const string testId);
    Test *getTestByTeacherId(const string &teacherId, int &count);
    bool deleteTestById(const string testId);
    bool checkPassword(const Test &test, const string &pass) const;
    bool createTest(const string &teacherId, const string &title, const int &totalQuestion, const string &password,
                    const int &duration, const string &startsAt, const string &endsAt);
    bool updateTest(const string &testId, const string &teacherId, const string &title, const int &totalQuestion,
                    const string &password, const int &duration, const string &startsAt, const string &endsAt);
    Test *getLastTest();
    int countIncomingTest(string id);
    int countRunningTest(string id);
    int countCompletedTest(string id);
    Test *getRunningTest(int &count);
};

#endif
