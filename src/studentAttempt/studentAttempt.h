#ifndef STUDENTATTEMPT_H
#define STUDENTATTEMPT_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

class StudentAttempt{
    private:
        string id;
        string testId;
        string studentId;
        string questionId[1000];  
        string studentAnswer;
        int correctAnswer;
        time_t startsAt;  
        time_t finishedAt;  

    public:
        StudentAttempt();
        StudentAttempt(string id, string testId, string studentId, string questionId[], string studentAnswer,
                    int correctAnswer, time_t startsAt, time_t finishedAt);
        ~StudentAttempt();
        
        string getId() const;
        string getTestId() const;
        string getStudentId() const;
        string getQuestionId(int index) const;  
        string getStudentAnswer() const;
        int getCorrectAnswer() const;
        string getStartsAt() const;
        string getFinishedAt() const;
};

class StudentAttemptManager {
private:
    static const int MAX_ATTEMPTS = 1000;
    StudentAttempt attempts[MAX_ATTEMPTS];
    int attemptCount;

    bool validateTestId(const string &testId) const;
    bool validateStudentId(const string &studentId) const;
    bool validateStudentAnswer(const string &studentAnswer) const;

public:
    StudentAttemptManager();
    void saveToFile() const;
    void loadFromFile();
    bool addAttempt(const StudentAttempt &attempt);
    int getAttemptCount() const;

    StudentAttempt* getAttemptById(const string &id);
    StudentAttempt* getAttemptByTestId(const string &testId);
    StudentAttempt* getAttemptByQuestionId(const string &questionId);
    
    bool createAttempt(const string &id, const string &testId, const string &studentId,
                const string &studentAnswer, const time_t startsAt, const time_t finishedAt);
};

#endif
