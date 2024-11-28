#include "question.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

void Question::formattedId(int id)
{
    std::stringstream tmp;
    tmp << "QUE" << setw(3) << setfill('0') << id;
    this->id = tmp.str();
}
Question::Question()
    : teacherId(""), chapterId(""), questionText(""), numberOfOptions(0), correctAnswerId(0)
{
}

Question::Question(const string &id, const string &teacherId, const string &chapterId,
                   const string &questionText, int numberOfOptions, const string options[],
                   int correctAnswerId)
    : id(id), teacherId(teacherId), chapterId(chapterId), questionText(questionText),
      numberOfOptions(numberOfOptions), correctAnswerId(correctAnswerId)
{
    for (int i = 0; i < numberOfOptions; ++i)
    {
        this->options[i] = options[i];
    }
}
const string &Question::getId() const
{
    return id;
}
void Question::setId(int id)
{
    formattedId(id);
}
void Question::setId(string id)
{
    this->id = id;
}
const string &Question::getTeacherId() const
{
    return teacherId;
}
void Question::setTeacherId(const string &teacherId)
{
    this->teacherId = teacherId;
}
const string &Question::getChapterId() const
{
    return chapterId;
}
void Question::setChapterId(const string &chapterId)
{
    this->chapterId = chapterId;
}
string Question::getQuestionText() const
{
    return questionText;
}
void Question::setQuestionText(const string &questionText)
{
    this->questionText = questionText;
}
string Question::getOption(int index) const
{
    if (index >= 0 && index < numberOfOptions)
    {
        return options[index];
    }
    return "";
}
void Question::setOption(int index, const string &option)
{
    if (index >= 0 && index < 100)
    {
        options[index] = option;
    }
}
int Question::getNumberOfOptions() const
{
    return numberOfOptions;
}
void Question::setNumberOfOptions(int numberOfOptions)
{
    this->numberOfOptions = numberOfOptions;
}
int Question::getCorrectAnswerId() const
{
    return correctAnswerId;
}
void Question::setCorrectAnswerId(int correctAnswerId)
{
    this->correctAnswerId = correctAnswerId;
}
bool QuestionBank::addQuestion(const string &teacherId, const string &chapterId,
                               const string &questionText, int numberOfOptions, const string options[],
                               int correctAnswerId)
{
    if (questionCount >= 1000)
    {
        return false;
    }
    if (teacherId.empty())
    {
        return false;
    }
    if (chapterId.empty())
    {
        return false;
    }
    if (questionText.empty())
    {
        return false;
    }
    if (numberOfOptions < 2)
    {
        return false;
    }
    for (int i = 0; i < numberOfOptions; ++i)
    {
        if (options[i].empty())
        {
            return false;
        }
    }
    if (correctAnswerId < 0 || correctAnswerId >= numberOfOptions)
    {
        return false;
    }
    Question newQuestion;
    newQuestion.setId(questionCount);
    newQuestion.setTeacherId(teacherId);
    newQuestion.setChapterId(chapterId);
    newQuestion.setQuestionText(questionText);
    newQuestion.setNumberOfOptions(numberOfOptions);
    newQuestion.setCorrectAnswerId(correctAnswerId);
    for (int i = 0; i < numberOfOptions; ++i)
    {
        newQuestion.setOption(i, options[i]);
    }
    questions[questionCount++] = newQuestion;
    // saveToFile();
    return true;
}
Question *QuestionBank::getQuestionById(const string &id)
{
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getId() == id)
        {
            return &questions[i];
        }
    }
    return nullptr;
}
bool QuestionBank::updateQuestion(const string &id, const string &teacherId, const string &chapterId,
                                  const string &questionText, int numberOfOptions, const string options[],
                                  int correctAnswerId)
{
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getId() == id)
        {
            questions[i].setTeacherId(teacherId);
            questions[i].setChapterId(chapterId);
            questions[i].setQuestionText(questionText);
            questions[i].setNumberOfOptions(numberOfOptions);
            for (int j = 0; j < numberOfOptions; ++j)
            {
                questions[i].setOption(j, options[j]);
            }
            questions[i].setCorrectAnswerId(correctAnswerId);
            return true;
        }
    }
    return false;
}
bool QuestionBank::deleteQuestion(const string &id)
{
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getId() == id)
        {
            questions[i].setId("");
            questions[i].setTeacherId("");
            questions[i].setChapterId("");
            questions[i].setQuestionText("");
            questions[i].setNumberOfOptions(0);
            for (int j = 0; j < 10; ++j)
            {
                questions[i].setOption(j, "");
            }
            questions[i].setCorrectAnswerId(-1);
            return true;
        }
    }
    return false;
}
int QuestionBank::getTotalQuestionByTeacherId(const string &teacherId) const
{
    int count = 0;
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getTeacherId() == teacherId)
        {
            ++count;
        }
    }
    return count;
}

Question *QuestionBank::getQuestionByTeacherId(const string &teacherId, int &foundCount) const
{
    Question *foundQuestions = new Question[questionCount];
    foundCount = 0;
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getTeacherId() == teacherId)
        {
            // qDebug() << "Found question:" << QString::fromStdString(questions[i].getId());
            foundQuestions[foundCount++] = questions[i];
        }
    }
    // qDebug() << "Found count:" << foundCount;
    return foundQuestions;
}

Question *QuestionBank::getAllQuestions() const
{
    return const_cast<Question *>(questions);
}

Question *QuestionBank::getQuestionByChapterId(const string &chapterId, int &foundCount, string teacherId) const
{
    Question *foundQuestions = new Question[questionCount];
    foundCount = 0;
    for (int i = 0; i < questionCount; ++i)
    {
        if (questions[i].getChapterId() == chapterId && questions[i].getTeacherId() == teacherId)
        {
            foundQuestions[foundCount++] = questions[i];
        }
    }
    return foundQuestions;
}

int QuestionBank::getNumberOfQuestion(string teacherId, string chapterId)
{
    int count = 0;
    for (int i = 0; i < questionCount; i++)
    {
        if (questions[i].getTeacherId() == teacherId && questions[i].getChapterId() == chapterId)
        {
            count++;
        }
    }
    return count;
}

void QuestionBank::loadFromFile()
{
    ifstream inFile("G:\\DUT\\pbl2-quizz\\src\\questionBank\\questions.txt");
    if (!inFile.is_open())
    {
        cout << "File not found" << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string id, teacherId, chapterId, questionText;
        string numberOfOptionsStr, correctAnswerIdStr;

        // Đọc các thông tin cơ bản của câu hỏi
        getline(ss, id, '|');
        getline(ss, teacherId, '|');
        getline(ss, chapterId, '|');
        getline(ss, questionText, '|');
        getline(ss, numberOfOptionsStr, '|');
        getline(ss, correctAnswerIdStr, '|');

        // Đọc các option
        string options[10];
        int numberOfOptions = stoi(numberOfOptionsStr);
        for (int i = 0; i < numberOfOptions; ++i)
        {
            getline(ss, options[i], '|');
        }
        string newQuestionText = "";
        for (int i = 0; i < questionText.length(); i++)
        {
            if (questionText[i] == '\\' && i + 1 < questionText.length() && questionText[i + 1] == 'n')
            {
                newQuestionText += '\n'; // Thêm ký tự xuống dòng
                i++;                     // Bỏ qua ký tự 'n'
            }
            else
            {
                newQuestionText += questionText[i]; // Thêm các ký tự khác
            }
        }
        int correctAnswerId = stoi(correctAnswerIdStr);
        // Tạo câu hỏi mới và thêm vào danh sách
        Question newQuestion(id, teacherId, chapterId, newQuestionText, numberOfOptions, options, correctAnswerId);
        questions[questionCount++] = newQuestion;
    }

    inFile.close();
}
void QuestionBank::saveToFile() const
{
    ofstream outFile("G:\\DUT\\pbl2-quizz\\src\\questionBank\\questions.txt");
    if (!outFile.is_open())
    {
        cout << "File not found" << endl;
        return;
    }

    for (int i = 0; i < questionCount; i++)
    {
        const Question &q = questions[i];
        string questionText = q.getQuestionText();
        string newQuestionText = "";

        for (char c : questionText)
        {
            if (c == '\n')
            {
                newQuestionText += "\\n"; // Thay thế ký tự '\n' bằng chuỗi "\\n"
            }
            else
            {
                newQuestionText += c;
            }
        }
        outFile << q.getId() << "|"
                << q.getTeacherId() << "|"
                << q.getChapterId() << "|"
                << newQuestionText << "|"
                << q.getNumberOfOptions() << "|"
                << q.getCorrectAnswerId() << "|";

        // Ghi các option, không thêm dấu '|' sau option cuối cùng
        for (int j = 0; j < q.getNumberOfOptions(); j++)
        {
            outFile << q.getOption(j) << "|";
        }

        // Kết thúc ghi 1 dòng
        outFile << "\n";
    }

    outFile.close();
}
QuestionBank::~QuestionBank()
{
    saveToFile();
}
QuestionBank::QuestionBank()
{
    loadFromFile();
}
