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
    int id;
    int teacherId;
    string title;
    int totalQuestion;
    string password;
    int duration;
    string startsAt;
    string endsAt;
    Status status;

public:
    Test();
    Test(int teacherId, string title, int totalQuestion, string password,
         int duration, string startsAt, string endsAt, Status status = INCOMING);
    ~Test();

    int getId() const;
    int getTeacherId() const;
    string getTitle() const;
    int getTotalQuestion() const;
    string getPassword() const;
    int getDuration() const;
    string getStartsAt() const;
    string getEndsAt() const;
    Status getStatus() const;

    void setId(int id);
    void setTeacherId(int teacherId);
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

    bool validateTeacherId(const int &teacherId) const;
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

    Test *getTestById(const int testId);
    bool checkPassword(const Test &test, const string &pass) const;
    bool createTest(const int &teacherId, const string &title, const int &totalQuestion, const string &password,
                    const int &duration, const string &startsAt, const string &endsAt);
    bool updateTest(const int &testId, const int &teacherId, const string &title, const int &totalQuestion,
                    const string &password, const int &duration, const string &startsAt, const string &endsAt);
    int countIncomingTest(int teacherId);
    int countRunningTest(int teacherId);
    int countCompletedTest(int teacherId);
    Test *getTestByTeacherId(const int &teacherId, int &count);
};

#endif
