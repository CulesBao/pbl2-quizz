#include <bits/stdc++.h>
#include "studentAttempt.h"
using namespace std;

//CONSTRUCTOR
StudentAttempt::StudentAttempt()
    : id(""), testId(""), studentId(""), studentAnswer(""), correctAnswer(0), startsAt(0), finishedAt(0) 
{
    memset(questionId, 0, sizeof(questionId)); 
}

StudentAttempt::StudentAttempt(string id, string testId, string studentId, string questionId[], string studentAnswer,
                               int correctAnswer, time_t startsAt, time_t finishedAt)
    : id(id), testId(testId), studentId(studentId), studentAnswer(studentAnswer), correctAnswer(correctAnswer), 
      startsAt(startsAt), finishedAt(finishedAt)
{
    for (int i = 0; i < 1000; ++i) {
        this->questionId[i] = questionId[i]; 
    }
}

StudentAttempt::~StudentAttempt() 
{
}

//CAC HAM GET
string StudentAttempt::getId() const { return id; }
string StudentAttempt::getTestId() const { return testId; }
string StudentAttempt::getStudentId() const { return studentId; }
string StudentAttempt::getQuestionId(int index) const { return questionId[index]; }
string StudentAttempt::getStudentAnswer() const { return studentAnswer; }
int StudentAttempt::getCorrectAnswer() const { return correctAnswer; }

string StudentAttempt::getStartsAt() const {
    struct tm* timeinfo;
    timeinfo = localtime(&startsAt);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", timeinfo);
    return string(buffer);
}

string StudentAttempt::getFinishedAt() const {
    struct tm* timeinfo;
    timeinfo = localtime(&finishedAt);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", timeinfo);
    return string(buffer);
}


StudentAttemptManager::StudentAttemptManager() : attemptCount(0) 
{
    loadFromFile();
}

//LUU VAO TEP
void StudentAttemptManager::saveToFile() const {
    ofstream outFile("attempts.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < attemptCount; ++i) {
            outFile << attempts[i].getId() << ","
                    << attempts[i].getTestId() << ","
                    << attempts[i].getStudentId() << ",";
            for (int j = 0; j < 1000 && attempts[i].getQuestionId(j) != ""; ++j) {
                outFile << attempts[i].getQuestionId(j) << " ";
            }
            outFile << ",";

            outFile << attempts[i].getStudentAnswer() << ","
                    << attempts[i].getCorrectAnswer() << ","
                    << attempts[i].getStartsAt() << ","
                    << attempts[i].getFinishedAt() << '\n';
        }
        outFile.close();
    }
}

//DOC TU TEP
void StudentAttemptManager::loadFromFile()
{
    ifstream inFile("attempts.txt");
    if (!inFile.is_open()) {
        return; 
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string id, testId, studentId, studentAnswer, correctAnswerStr, startsAt, finishedAt;
        array<string, 1000> questionId;

        if (!getline(ss, id, ',') || 
            !getline(ss, testId, ',') || 
            !getline(ss, studentId, ',') || 
            !getline(ss, questionId[0], ',') || 
            !getline(ss, studentAnswer, ',') || 
            !getline(ss, correctAnswerStr, ',') || 
            !getline(ss, startsAt, ',') || 
            !getline(ss, finishedAt)) {
            continue; 
        }

        int i = 0;
        while (getline(ss, questionId[i], ',') && i < 1000) {
            ++i;
        }
        int correctAnswer = stoi(correctAnswerStr);

        struct tm tmStart = {};
        istringstream startStream(startsAt);
        startStream >> get_time(&tmStart, "%H:%M:%S %d/%m/%Y");

        struct tm tmFinish = {};
        istringstream finishStream(finishedAt);
        finishStream >> get_time(&tmFinish, "%H:%M:%S %d/%m/%Y");

        time_t startTime = mktime(&tmStart);
        time_t finishTime = mktime(&tmFinish);

        attempts[attemptCount] = StudentAttempt(id, testId, studentId, questionId.data(), studentAnswer, correctAnswer, 
                                                startTime, finishTime);
        attemptCount++;
    }

    inFile.close();
}

//THEM MOT BAI THI
bool StudentAttemptManager::addAttempt(const StudentAttempt &attempt)   
{
    if (attemptCount < MAX_ATTEMPTS) {
        attempts[attemptCount++] = attempt;
        return true;
    }
    return false;
}

//LAY SO LUONG BAI THI
int StudentAttemptManager::getAttemptCount() const { return attemptCount; }

//LAY BAI THI THEO ID
StudentAttempt* StudentAttemptManager::getAttemptById(const string &id) {
    for (int i = 0; i < attemptCount; ++i) {
        if (attempts[i].getId() == id) {
            return &attempts[i];
        }
    }
    return nullptr;
}

//LAY BAI THI THEO ID BAI KIEM TRA
StudentAttempt* StudentAttemptManager::getAttemptByTestId(const string &testId) {
    for (int i = 0; i < attemptCount; ++i) {
        if (attempts[i].getTestId() == testId) {
            return &attempts[i];
        }
    }
    return nullptr;
}

//LAY BAI THI THEO ID CAU HOI
StudentAttempt* StudentAttemptManager::getAttemptByQuestionId(const string &questionId) {
    for (int i = 0; i < attemptCount; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if (attempts[i].getQuestionId(j) == questionId) {
                return &attempts[i];
            }
        }
    }
    return nullptr;
}

//CAC HAM VALIDATE
bool StudentAttemptManager::validateTestId(const string &testId) const {
    regex pattern("^TID\\d{3}$");
    return regex_match(testId, pattern);
}

bool StudentAttemptManager::validateStudentId(const string &studentId) const {
    regex pattern("^STD\\d{3}$");
    return regex_match(studentId, pattern);
}

bool StudentAttemptManager::validateStudentAnswer(const string &studentAnswer) const {
    return true;
}


//TAO BAI THI
bool StudentAttemptManager::createAttempt(const string &id, const string &testId, const string &studentId,
                                          const string &studentAnswer, const time_t startsAt, const time_t finishedAt) {
    if (!validateTestId(testId) || !validateStudentId(studentId)) {
        return false; 
    }

    string questionIds[] = {"Q1", "Q2", "Q3"};

    time_t currentTime = time(nullptr);
    time_t startTime = (startsAt != 0) ? startsAt : currentTime;
    time_t finishTime = (finishedAt != 0) ? finishedAt : (startTime + 3600);

    StudentAttempt newAttempt(id, testId, studentId, questionIds, studentAnswer, 4, startTime, finishTime);
    addAttempt(newAttempt);
    saveToFile();
    return true;
}