#ifndef CHAPTER_H
#define CHAPTER_H

#include <string>
using namespace std;

class Chapter
{
private:
    string id;
    string name;

public:
    Chapter(int id, const string &name = "");
    Chapter();
    ~Chapter();
    void formattedId(int id);
    const string &getId() const;
    const string &getName() const;
    void setName(const string &newName);
    void setId(int id);
    bool setId(const string &id);
    static void resetCurrentId();
};

class ChapterManager
{
private:
    Chapter chapters[500];
    static int chapterCount;
    void loadFromFile();
    void saveToFile() const;

public:
    ChapterManager();
    ~ChapterManager();
    int getChapterCount() const;
    bool addChapter(const string &name);
    const Chapter *getAllChapters() const;
    Chapter *getChapterById(const int id);
    bool updateChapter(const string &id, const string &newName);
    bool deleteChapter(const string &id);
    string getChapterIdByName(const string &name);
};

#endif // CHAPTER_H
