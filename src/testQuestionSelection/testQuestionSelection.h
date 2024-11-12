#ifndef TestQuestionSelection_h
#define TestQuestionSelection_h
#include <iostream>
using namespace std;

class TestQuestionSelection
{
private:
    string id;
    string testId;
    string chapterId;
    int numberOfQuestions;

public:
    TestQuestionSelection(/* args */);
    ~TestQuestionSelection();
    bool setTestId(string testId);
    bool setChapterId(string chapterId);
    bool setNumberOfQuestions(int numberOfQuestions);
    bool setId(int id);
    bool setId(string id);
    string getTestId();
    string getChapterId();
    int getNumberOfQuestions();
    string getId();
};

class TestQuestionSelectionManager
{
private:
    TestQuestionSelection manager[1000];
    static int count;

public:
    TestQuestionSelectionManager(/* args */);
    ~TestQuestionSelectionManager();
    bool addTestQuestionSelection(string testId, string chapterId, int numberOfQuestions);
    bool deleteTestQuestionSelection(string id);
    bool updateTestQuestionSelection(string id, string testId, string chapterId, int numberOfQuestions);
    TestQuestionSelection getTestQuestionSelection(string id);
    TestQuestionSelection *getAllTestQuestionSelection();
    TestQuestionSelection *getTestQuestionSelectionByTestId(string testId, int &count);
    int getCount();
    void saveToFile();
    void loadFromFile();
};

#endif