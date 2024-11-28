#include "chapter.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
Chapter::Chapter(int id, const string &name) : name(name)
{
    formattedId(id);
}
Chapter::Chapter()
{
}

const string &Chapter::getId() const
{
    return id;
}

const string &Chapter::getName() const
{
    return name;
}

void Chapter::setName(const string &newName)
{
    name = newName;
}

void Chapter::setId(int id)
{
    formattedId(id);
}
void Chapter::formattedId(int id)
{
    stringstream tmp;
    tmp << "CHA" << setw(3) << setfill('0') << id;
    this->id = tmp.str();
}
bool Chapter::setId(const string &id)
{
    if (id.length() != 6)
    {
        return false;
    }
    if (id.substr(0, 3) != "CHA")
    {
        return false;
    }
    for (int i = 3; i < 6; ++i)
    {
        if (!isdigit(id[i]))
        {
            return false;
        }
    }
    this->id = id;
    return true;
}
Chapter::~Chapter()
{
}
ChapterManager::ChapterManager()
{
    if (chapterCount != 0)
        chapterCount = 0;
    loadFromFile();
}

ChapterManager::~ChapterManager()
{
    saveToFile();
}
int ChapterManager::chapterCount = 0;
void ChapterManager::loadFromFile()
{
    ifstream inputFile("G:\\DUT\\pbl2-quizz\\src\\chapter\\chapters.txt");
    if (!inputFile)
    {
        return;
    }
    string name;
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name, '|');
        Chapter tmp;
        tmp.setId(id);
        tmp.setName(name);
        chapters[chapterCount++] = tmp;
    }
}

void ChapterManager::saveToFile() const
{
    ofstream outputFile("G:\\DUT\\pbl2-quizz\\src\\chapter\\chapters.txt");
    if (!outputFile)
    {
        return;
    }
    for (int i = 0; i < chapterCount; ++i)
    {
        if (chapters[i].getId().empty())
        {
            continue;
        }
        outputFile << chapters[i].getId() << "|" << chapters[i].getName() << "|" << endl;
    }
}

bool ChapterManager::addChapter(const string &name)
{
    if (name.empty() || name.length() < 3)
    {
        return false;
    }
    if (chapterCount >= 500)
    {
        return false;
    }
    for (int i = 0; i < chapterCount; ++i)
    {
        if (chapters[i].getName() == name)
        {
            return false;
        }
    }
    chapters[chapterCount++] = Chapter(chapterCount, name);
    return true;
}

const Chapter *ChapterManager::getAllChapters() const
{
    return chapters;
}

Chapter *ChapterManager::getChapterById(const int id)
{
    if (id >= 0 && id < chapterCount)
    {
        return &chapters[id];
    }
    return nullptr;
}

bool ChapterManager::updateChapter(const string &id, const string &newName)
{
    for (int i = 0; i < chapterCount; ++i)
    {
        if (chapters[i].getId() == id)
        {
            chapters[i].setName(newName);
            return true;
        }
    }
    return false;
}

bool ChapterManager::deleteChapter(const string &id)
{
    for (int i = 0; i < chapterCount; ++i)
    {
        if (chapters[i].getId() == id)
        {
            chapters[i].setId(0);
            chapters[i].setName("");
            return true;
        }
    }
    return false;
}
int ChapterManager::getChapterCount() const
{
    return chapterCount;
}
string ChapterManager::getChapterIdByName(const string &name)
{
    for (int i = 0; i < chapterCount; i++)
    {
        if (chapters[i].getName() == name)
        {
            return chapters[i].getId();
        }
    }
    return "";
}
string ChapterManager::getChapterNameById(const string &id)
{
    for (int i = 0; i < chapterCount; i++)
    {
        if (chapters[i].getId() == id)
        {
            return chapters[i].getName();
        }
    }
    return "";
}