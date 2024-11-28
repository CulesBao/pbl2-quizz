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
    ifstream file("G:\\DUT\\pbl2-quizz\\src\\testQuestionSelection\\TestQuestionSelection.txt");
    if (!file.is_open()) // Kiểm tra nếu file không mở được
    {
        cout << "File not found or could not be opened!" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) // Bỏ qua các dòng trống
            continue;

        stringstream ss(line);
        string id, testId, chapterId, numberOfQuestions;

        if (getline(ss, id, '|') &&
            getline(ss, testId, '|') &&
            getline(ss, chapterId, '|') &&
            getline(ss, numberOfQuestions))
        {
            // Kiểm tra và thêm TestQuestionSelection
            addTestQuestionSelection(testId, chapterId, stoi(numberOfQuestions));
        }
    }
    file.close();
}
int TestQuestionSelectionManager::count = 0;
TestQuestionSelectionManager::TestQuestionSelectionManager(/* args */)
{
    if (count != 0)
        count = 0;
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
    if (testId.empty() || chapterId.empty() || numberOfQuestions <= 0)
    {
        cout << "Invalid input data. Skipping addition." << endl;
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
            manager[i].setId("");
            manager[i].setTestId("");
            manager[i].setChapterId("");
            manager[i].setNumberOfQuestions(0);
            saveToFile(); // Lưu lại dữ liệu sau khi xóa
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
            saveToFile(); // Lưu lại dữ liệu sau khi cập nhật
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
    for (int i = 0; i < TestQuestionSelectionManager::count; i++)
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
    ofstream file("G:\\DUT\\pbl2-quizz\\src\\testQuestionSelection\\TestQuestionSelection.txt");
    if (!file.is_open()) // Kiểm tra file có mở được không
    {
        cout << "Unable to open file for saving." << endl;
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (manager[i].getId().empty())
            continue;
        file << manager[i].getId() << "|"
             << manager[i].getTestId() << "|"
             << manager[i].getChapterId() << "|"
             << manager[i].getNumberOfQuestions() << endl;
    }
    file.close();
}