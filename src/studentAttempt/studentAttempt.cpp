#include <bits/stdc++.h>
#include <ctime>
#include <random>
#include <iomanip>
#include <sstream>
#include "studentAttempt.h"
#include "../questionBank/question.h"
#include "../testQuestionSelection/testQuestionSelection.h"
#include "../questionBank/question.h"
#include <QDebug>
using namespace std;

// CONSTRUCTOR
StudentAttempt::StudentAttempt()
{
    // memset(questionId, 0, sizeof(questionId));
}
void StudentAttempt::formatId(int id)
{
    stringstream ss;
    ss << "STA" << setfill('0') << setw(3) << id;
    this->id = ss.str();
}

void StudentAttempt::generateQuestionId(string teacherId)
{
    int isNotEnough = 0;
    QuestionBank questionBank;
    TestQuestionSelectionManager testQuestionSelectionManager;
    int testQuestionSelectionCount = 0;

    // Lấy danh sách chương theo testId
    TestQuestionSelection *testQuestionSelectionByTestId =
        testQuestionSelectionManager.getTestQuestionSelectionByTestId(testId, testQuestionSelectionCount);

    if (testQuestionSelectionCount == 0)
    {
        std::cerr << "Khong tim thay " << testId << std::endl;
        return;
    }

    for (int i = 0; i < testQuestionSelectionCount; i++)
    {
        int questionCount = 0;
        Question *questionByChapterId = questionBank.getQuestionByChapterId(
            testQuestionSelectionByTestId[i].getChapterId(), questionCount, teacherId);

        if (!questionByChapterId || questionCount == 0)
        {
            std::cerr << "Không có câu hỏi trong chương " << testQuestionSelectionByTestId[i].getChapterId() << std::endl;
            continue;
        }

        vector<Question> questionsByChapterId(questionByChapterId, questionByChapterId + questionCount);
        int requiredQuestions = testQuestionSelectionByTestId[i].getNumberOfQuestions();

        if (questionCount < requiredQuestions)
        {
            isNotEnough += requiredQuestions - questionCount;
            for (int j = 0; j < questionCount; j++)
            {
                questionId.push_back(questionsByChapterId[j].getId());
            }
            while (questionId.size() < requiredQuestions)
            {
                questionId.push_back(questionsByChapterId[questionId.size() % questionCount].getId());
            }
        }
        else
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            for (int j = questionCount - 1; j > 0; j--)
            {
                std::uniform_int_distribution<> dis(0, j);
                int randomIndex = dis(gen);
                std::swap(questionsByChapterId[j], questionsByChapterId[randomIndex]);
            }

            for (int j = 0; j < requiredQuestions; j++)
            {
                questionId.push_back(questionsByChapterId[j].getId());
            }
        }
    }

    if (isNotEnough > 0)
    {
        std::cerr << "Thiếu tổng cộng " << isNotEnough << " câu hỏi cho bài kiểm tra!" << std::endl;
    }

    int totalQuestions = questionId.size();
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = totalQuestions - 1; i > 0; i--)
    {
        std::uniform_int_distribution<> dis(0, i);
        int randomIndex = dis(gen);
        std::swap(questionId[i], questionId[randomIndex]);
    }

    delete[] testQuestionSelectionByTestId; // Giải phóng nếu cần
}

StudentAttempt::StudentAttempt(int id, string testId, string studentId, int totalQuestions, int time, string teacherId)
{
    formatId(id);
    this->testId = testId;
    this->studentId = studentId;
    this->totalQuestions = totalQuestions;
    this->startsAt = std::time(nullptr);
    this->finishedAt = std::time(nullptr);
    this->time = time;
    srand(std::time(0));
    generateQuestionId(teacherId);
    for (int i = 0; i < totalQuestions; i++)
    {
        studentAnswer[i] = 0;
    }
    correctAnswer = 0;
}

StudentAttempt::~StudentAttempt()
{
}

// CAC HAM GET
string StudentAttempt::getId() const { return id; }
string StudentAttempt::getTestId() const { return testId; }
string StudentAttempt::getStudentId() const { return studentId; }
string StudentAttempt::getQuestionId(int index) const { return questionId[index]; }
int StudentAttempt::getCorrectAnswer() const { return correctAnswer; }

string StudentAttempt::getStartsAt() const
{
    struct tm *timeinfo;
    timeinfo = localtime(&startsAt);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", timeinfo);
    return string(buffer);
}

string StudentAttempt::getFinishedAt() const
{
    struct tm *timeinfo;
    timeinfo = localtime(&finishedAt);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", timeinfo);
    return string(buffer);
}
void StudentAttempt::setEndsAt()
{
    finishedAt = std::time(nullptr);
}
void StudentAttempt::setStudentAnswer(int index, int studentAnswer)
{
    this->studentAnswer[index] = studentAnswer;
}
int StudentAttempt::getSudentAnswer(int index) const
{
    return studentAnswer[index];
}
void StudentAttempt::setCorrectAnswer()
{
    QuestionBank questionBank;
    int correct = 0;
    for (int i = 0; i < totalQuestions; i++)
    {
        Question *question = questionBank.getQuestionById(questionId[i]);
        if (question->getCorrectAnswerId() == studentAnswer[i])
        {
            correct++;
        }
    }
    correctAnswer = correct;
}
bool StudentAttempt::setCorretAnswer(int correctAnswer)
{
    this->correctAnswer = correctAnswer;
    return true;
}

StudentAttemptManager::StudentAttemptManager() : attemptCount(0)
{
    loadFromFile();
}
int StudentAttempt::getTotalQuestions() const
{
    return totalQuestions;
}

// LUU VAO TEP
void StudentAttemptManager::saveToFile() const
{
    ofstream outFile("G:\\DUT\\pbl2-quizz\\src\\studentAttempt\\studentAttempt.txt");
    if (outFile.is_open())
    {
        for (int i = 0; i < attemptCount; ++i)
        {
            StudentAttempt attempt = attempts[i];
            attempt.setCorrectAnswer();
            outFile << attempts[i].getId() << "|"
                    << attempts[i].getTestId() << "|"
                    << attempts[i].getStudentId() << "|"
                    << attempts[i].getTotalQuestions() << "|";
            for (int j = 0; j < attempts[i].getTotalQuestions(); ++j)
            {
                outFile << attempts[i].getQuestionId(j) << "|";
            }

            for (int j = 0; j < attempts[i].getTotalQuestions(); ++j)
                outFile << attempts[i].getStudentAnswer(j) << "|";

            outFile << attempts[i].getCorrectAnswer() << "|"
                    << attempts[i].getStartsAt() << "|"
                    << attempts[i].getFinishedAt() << '\n';
        }
        outFile.close();
    }
}
void StudentAttemptManager::loadFromFile()
{
    ifstream inFile("G:\\DUT\\pbl2-quizz\\src\\studentAttempt\\studentAttempt.txt");
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string id, testId, studentId, studentAnswer, startsAtStr, finishedAtStr;
            int totalQuestions, correctAnswer;
            time_t startsAt, finishedAt;
            vector<string> questionIds;
            vector<int> studentAnswers;

            getline(ss, id, '|');
            getline(ss, testId, '|');
            getline(ss, studentId, '|');
            ss >> totalQuestions;
            ss.ignore(1, '|');

            for (int i = 0; i < totalQuestions; ++i)
            {
                string questionId;
                getline(ss, questionId, '|');
                questionIds.push_back(questionId);
            }

            for (int i = 0; i < totalQuestions; ++i)
            {
                int answer;
                ss >> answer;
                ss.ignore(1, '|');
                studentAnswers.push_back(answer);
            }

            ss >> correctAnswer;
            ss.ignore(1, '|');
            getline(ss, startsAtStr, '|');
            getline(ss, finishedAtStr, '|');

            struct tm tm;
            std::istringstream startsAtStream(startsAtStr);
            startsAtStream >> std::get_time(&tm, "%H:%M:%S %d/%m/%Y");
            startsAt = mktime(&tm);

            std::istringstream finishedAtStream(finishedAtStr);
            finishedAtStream >> std::get_time(&tm, "%H:%M:%S %d/%m/%Y");
            finishedAt = mktime(&tm);

            StudentAttempt attempt;
            attempt.setId(id);
            attempt.setTestId(testId);
            attempt.setStudentId(studentId);
            attempt.setTotalQuestions(totalQuestions);
            for (int i = 0; i < totalQuestions; ++i)
            {
                attempt.setStudentAnswer(i, studentAnswers[i]);
                attempt.setQuestionId(questionIds[i]);
            }
            attempt.setStartsAt(startsAt);
            attempt.setFinishedAt(finishedAt);
            attempt.setCorretAnswer(correctAnswer);

            attempts[attemptCount++] = attempt;
        }
        inFile.close();
    }
}
void StudentAttempt::setQuestionId(string questionId)
{
    this->questionId.push_back(questionId);
}
int StudentAttempt::getStudentAnswer(int index) const
{
    return studentAnswer[index];
}
void StudentAttempt::setId(string id)
{
    this->id = id;
}
void StudentAttempt::setTestId(string testId)
{
    this->testId = testId;
}
void StudentAttempt::setStudentId(string studentId)
{
    this->studentId = studentId;
}
void StudentAttempt::setTotalQuestions(int totalQuestions)
{
    this->totalQuestions = totalQuestions;
}
void StudentAttempt::setStartsAt(time_t startsAt)
{
    this->startsAt = startsAt;
}
void StudentAttempt::setFinishedAt(time_t finishedAt)
{
    this->finishedAt = finishedAt;
}

// THEM MOT BAI THI
bool StudentAttemptManager::addAttempt(const StudentAttempt &attempt)
{
    if (attemptCount < MAX_ATTEMPTS)
    {
        attempts[attemptCount++] = attempt;
        return true;
    }
    return false;
}

// LAY SO LUONG BAI THI
int StudentAttemptManager::getAttemptCount() const { return attemptCount; }

// LAY BAI THI THEO ID
StudentAttempt *StudentAttemptManager::getAttemptById(const string &id)
{
    for (int i = 0; i < attemptCount; ++i)
    {
        if (attempts[i].getId() == id)
        {
            return &attempts[i];
        }
    }
    return nullptr;
}
StudentAttempt StudentAttemptManager::operator[](int index)
{
    return attempts[index];
}
StudentAttempt *StudentAttemptManager::getAttemptsByStudentId(const string &studentId, int &foundCount) const
{
    StudentAttempt *foundAttempts = new StudentAttempt[MAX_ATTEMPTS];
    foundCount = 0;
    for (int i = 0; i < attemptCount; ++i)
    {
        if (attempts[i].getStudentId() == studentId)
        {
            foundAttempts[foundCount++] = attempts[i];
        }
    }
    return foundAttempts;
}

// LAY BAI THI THEO ID BAI KIEM TRA
StudentAttempt *StudentAttemptManager::getAttemptByTestId(const string &testId, int &foundCount) const
{
    StudentAttempt *foundAttempts = new StudentAttempt[MAX_ATTEMPTS];
    foundCount = 0;
    for (int i = 0; i < attemptCount; ++i)
    {
        if (attempts[i].getTestId() == testId)
        {
            foundAttempts[foundCount++] = attempts[i];
        }
    }
    return foundAttempts;
}

// LAY BAI THI THEO ID CAU HOI
StudentAttempt *StudentAttemptManager::getAttemptByQuestionId(const string &questionId)
{
    for (int i = 0; i < attemptCount; ++i)
    {
        for (int j = 0; j < 1000; ++j)
        {
            if (attempts[i].getQuestionId(j) == questionId)
            {
                return &attempts[i];
            }
        }
    }
    return nullptr;
}

// CAC HAM VALIDATE
bool StudentAttemptManager::validateTestId(const string &testId) const
{
    regex pattern("^TID\\d{3}$");
    return regex_match(testId, pattern);
}

bool StudentAttemptManager::validateStudentId(const string &studentId) const
{
    regex pattern("^STD\\d{3}$");
    return regex_match(studentId, pattern);
}

bool StudentAttemptManager::validateStudentAnswer(const string &studentAnswer) const
{
    return true;
}

// TAO BAI THI
StudentAttempt *StudentAttemptManager::createAttempt(const string &testId, const string &studentId, int totalQuestion, int time, string teacherId)
{
    if (!validateTestId(testId) || !validateStudentId(studentId))
    {
        return nullptr;
    }
    StudentAttempt newAttempt(attemptCount, testId, studentId, totalQuestion, time, teacherId);
    addAttempt(newAttempt);
    saveToFile();
    return &attempts[attemptCount - 1];
}
bool StudentAttemptManager::setStudentAnswer(StudentAttempt *attempt, int index, int studentAnswer)
{
    attempt->setStudentAnswer(index, studentAnswer);
    attempt->setCorrectAnswer();
    saveToFile();
    return true;
}
StudentAttemptManager::~StudentAttemptManager()
{
}
void StudentAttemptManager::setFinishedAtForLastAttempt()
{
    attempts[attemptCount - 1].setEndsAt();
    saveToFile();
}
