#ifndef STUDENTFORM_H
#define STUDENTFORM_H
#include <QWidget>
#include "QLCDNumber"
#include "./src/currentUser/currentUser.h"
#include "./src/testInfo/testInfo.h"
#include "./src/studentAttempt/studentAttempt.h"
#include "./src/questionBank/question.h"
#include "./src/student/student.h"
namespace Ui
{
    class StudentForm;
}

class StudentForm : public QWidget
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = nullptr);
    ~StudentForm();

private slots:
    void on_btnDashboard_clicked();
    void on_btnLogoutTeacherDashboard_clicked();
    void CountdownTimer(int);
    void setUpTestForm(Test test);
    void setHistoryTable();

    void on_pushButton_clicked();

    void on_btnEditProfileSubmit_clicked();

    void on_btnEditProfile_clicked();

    void on_btnAddNewTest_clicked();

private:
    int timeRemaining;
    Ui::StudentForm *ui;
    currentUser logged;
    TestManager testManager;
    studentManager managerStudent;
    StudentAttemptManager studentAttemptManager;
    QuestionBank questionBankManager;
    QTimer *timer;
    QLCDNumber *lcdDisplay;
};

#endif // STUDENTFORM_H
