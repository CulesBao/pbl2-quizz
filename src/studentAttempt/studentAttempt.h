#ifndef STUDENTATTEMPT_H
#define STUDENTATTEMPT_H

#include <iostream>
#include <string>
#include <ctime>
#include "../questionBank/question.h"
#include <sstream>
#include <vector>

using namespace std;

class StudentAttempt
{
private:
    string id;
    string testId;
    string studentId;
    int studentAnswer[100];
    vector<string> questionId;
    int totalQuestions;
    int correctAnswer;
    int time;
    time_t startsAt;
    time_t finishedAt;
    void formatId(int id);
    void generateQuestionId();

public:
    StudentAttempt();
    StudentAttempt(int id, string testId, string studentId, int totalQuestions, int time);
    ~StudentAttempt();

    string getId() const;
    string getTestId() const;
    string getStudentId() const;
    string getQuestionId(int index) const;
    int getCorrectAnswer() const;
    string getStartsAt() const;
    string getFinishedAt() const;
    void setEndsAt();
    void setStudentAnswer(int index, int studentAnswer);
    int getSudentAnswer(int index) const;
    void setCorrectAnswer();
    int getTotalQuestions() const;
    int getStudentAnswer(int index) const;
    void setId(string id);
    void setTestId(string testId);
    void setStudentId(string studentId);
    void setTotalQuestions(int totalQuestions);
    void setStartsAt(time_t startsAt);
    void setFinishedAt(time_t finishedAt);
    void setQuestionId(string questionId);
};

class StudentAttemptManager
{
private:
    static const int MAX_ATTEMPTS = 10;
    StudentAttempt attempts[MAX_ATTEMPTS];
    int attemptCount;

    bool validateTestId(const string &testId) const;
    bool validateStudentId(const string &studentId) const;
    bool validateStudentAnswer(const string &studentAnswer) const;

public:
    StudentAttemptManager();
    ~StudentAttemptManager();
    void saveToFile() const;
    void loadFromFile();
    bool addAttempt(const StudentAttempt &attempt);
    int getAttemptCount() const;

    StudentAttempt *getAttemptsByStudentId(const string &studentId, int &foundCount) const;
    StudentAttempt *getAttemptById(const string &id);
    StudentAttempt *getAttemptByTestId(const string &testId);
    StudentAttempt *getAttemptByQuestionId(const string &questionId);
    bool setStudentAnswer(StudentAttempt *attempt, int index, int studentAnswer);
    StudentAttempt *createAttempt(const string &testId, const string &studentId, int totalQuestion, int time);
};

#endif
