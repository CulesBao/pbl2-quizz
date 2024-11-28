#ifndef QUESTION_H
#define QUESTION_H
#include <string>
using namespace std;
class Question
{
private:
    string id;
    string teacherId;
    string chapterId;
    string questionText;
    int numberOfOptions;
    string options[10];
    int correctAnswerId;
    void formattedId(int id);

public:
    Question();
    Question(const string &id, const string &teacherId, const string &chapterId,
             const string &questionText, int numberOfOptions, const string options[],
             int correctAnswerId);
    const string &getId() const;
    void setId(int id);
    void setId(string id);
    const string &getTeacherId() const;
    void setTeacherId(const string &teacherId);
    const string &getChapterId() const;
    void setChapterId(const string &chapterId);
    string getQuestionText() const;
    void setQuestionText(const string &questionText);
    string getOption(int index) const;
    void setOption(int index, const string &option);
    int getNumberOfOptions() const;
    void setNumberOfOptions(int numberOfOptions);
    int getCorrectAnswerId() const;
    void setCorrectAnswerId(int correctAnswerId);
};

class QuestionBank
{
private:
    Question questions[1000];
    int questionCount = 0;

public:
    bool addQuestion(const string &teacherId, const string &chapterId,
                     const string &questionText, int numberOfOptions, const string options[],
                     int correctAnswerId);
    QuestionBank();
    Question *getQuestionById(const string &id);
    bool updateQuestion(const string &id, const string &teacherId, const string &chapterId,
                        const string &questionText, int numberOfOptions, const string options[],
                        int correctAnswerId);
    bool deleteQuestion(const string &id);
    int getTotalQuestionByTeacherId(const string &teacherId) const;
    Question *getQuestionByTeacherId(const string &teacherId, int &foundCount) const;
    Question *getAllQuestions() const;
    void loadFromFile();
    void saveToFile() const;
    Question *getQuestionByChapterId(const string &chapterId, int &foundCount, string teacherId) const;
    int getNumberOfQuestion(string teacherId, string chapterId);
    ~QuestionBank();
};

#endif // QUESTION_H
