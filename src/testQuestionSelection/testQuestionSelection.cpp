#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "TestQuestionSelection.h"
using namespace std;

TestQuestionSelection::TestQuestionSelection(/* args */)
{
}
TestQuestionSelection::~TestQuestionSelection()
{
}
bool TestQuestionSelection::setTestId(string testId)
{
    this->testId = testId;
    return true;
}
bool TestQuestionSelection::setChapterId(string chapterId)
{
    if (chapterId.length() == 0)
    {
        return false;
    }
    this->chapterId = chapterId;
    return true;
}
bool TestQuestionSelection::setNumberOfQuestions(int numberOfQuestions)
{
    if (numberOfQuestions < 0)
    {
        return false;
    }
    this->numberOfQuestions = numberOfQuestions;
    return true;
}
bool TestQuestionSelection::setId(int id)
{
    stringstream tmp;
    tmp << "TQS" << setw(3) << setfill('0') << id;
    this->id = tmp.str();
    return true;
}
bool TestQuestionSelection::setId(string id)
{
    this->id = id;
    return true;
}
string TestQuestionSelection::getTestId()
{
    return testId;
}
string TestQuestionSelection::getChapterId()
{
    return chapterId;
}
int TestQuestionSelection::getNumberOfQuestions()
{
    return numberOfQuestions;
}
string TestQuestionSelection::getId()
{
    return id;
}

void TestQuestionSelectionManager::loadFromFile()
{
    ifstream file;
    file.open("TestQuestionSelection.txt");
    if (!file)
    {
        cout << "File not found" << endl;
        return;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, testId, chapterId, numberOfQuestions;
        getline(ss, id, '|');
        getline(ss, testId, '|');
        getline(ss, chapterId, '|');
        getline(ss, numberOfQuestions);
        addTestQuestionSelection(testId, chapterId, stoi(numberOfQuestions));
    }
    file.close();
}
int TestQuestionSelectionManager::count = 0;
TestQuestionSelectionManager::TestQuestionSelectionManager(/* args */)
{
    loadFromFile();
}
TestQuestionSelectionManager::~TestQuestionSelectionManager()
{
    saveToFile();
}
bool TestQuestionSelectionManager::addTestQuestionSelection(string testId, string chapterId, int numberOfQuestions)
{
    if (count >= 1000)
    {
        return false;
    }
    TestQuestionSelection testQuestionSelection;
    testQuestionSelection.setTestId(testId);
    testQuestionSelection.setChapterId(chapterId);
    testQuestionSelection.setNumberOfQuestions(numberOfQuestions);
    testQuestionSelection.setId(count);
    manager[count] = testQuestionSelection;
    count++;
    return true;
}
bool TestQuestionSelectionManager::deleteTestQuestionSelection(string id)
{
    for (int i = 0; i < count; i++)
    {
        if (manager[i].getId() == id)
        {
            manager[i].setId("x");
            manager[i].setTestId("x");
            manager[i].setChapterId("x");
            manager[i].setNumberOfQuestions(0);
            return true;
        }
    }
    return false;
}
bool TestQuestionSelectionManager::updateTestQuestionSelection(string id, string testId, string chapterId, int numberOfQuestions)
{
    for (int i = 0; i < count; i++)
    {
        if (manager[i].getId() == id)
        {
            manager[i].setTestId(testId);
            manager[i].setChapterId(chapterId);
            manager[i].setNumberOfQuestions(numberOfQuestions);
            return true;
        }
    }
    return false;
}
TestQuestionSelection TestQuestionSelectionManager::getTestQuestionSelection(string id)
{
    for (int i = 0; i < count; i++)
    {
        if (manager[i].getId() == id)
        {
            return manager[i];
        }
    }
    return TestQuestionSelection();
}
TestQuestionSelection *TestQuestionSelectionManager::getAllTestQuestionSelection()
{
    return manager;
}
int TestQuestionSelectionManager::getCount()
{
    return count;
}
TestQuestionSelection *TestQuestionSelectionManager::getTestQuestionSelectionByTestId(string testId, int &count)
{
    TestQuestionSelection *result = new TestQuestionSelection[1000];
    count = 0;
    for (int i = 0; i < count; i++)
    {
        if (manager[i].getTestId() == testId)
        {
            result[count] = manager[i];
            count++;
        }
    }
    return result;
}

void TestQuestionSelectionManager::saveToFile()
{
    ofstream file;
    file.open("TestQuestionSelection.txt");
    for (int i = 0; i < count; i++)
    {
        file << manager[i].getId() << "|" << manager[i].getTestId() << "|" << manager[i].getChapterId() << "|" << manager[i].getNumberOfQuestions() << endl;
    }
    file.close();
}