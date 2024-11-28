#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "./src/teacher/teacher.h"
#include "./src/student/student.h"
#include "./src/testInfo/testInfo.h"
#include "./src/currentUser/currentUser.h"
#include "./src/chapter/chapter.h"
#include "./src/testQuestionSelection/testQuestionSelection.h"
#include "./src/questionBank/question.h"
#include "./src/studentAttempt/studentAttempt.h"
#include "./studentform.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    teacherManager managerTeacher;
    studentManager managerStudent;
    TestManager managerTest;
    currentUser logged = currentUser("", "", "", "", "");
    ChapterManager chapterManager;
    TestQuestionSelectionManager testQuestionSelectionManager;
    QuestionBank questionBankManager;
    StudentAttemptManager studentAttemptManager;
private slots:
    void login();
    void on_btnRegister_clicked();
    void on_btnRegister_2_clicked();
    void on_btnBack_clicked();
    void on_btnLogoutTeacherDashboard_clicked();
    void setUpTeacherDashboard();
    void on_btnDashboard_clicked();
    void on_btnAddNewTest_clicked();
    void on_btnAddNewTextNext_clicked();
    void on_btnEditProfile_clicked();
    void on_btnEditProfileSubmit_clicked();
    void on_btnMyQuestionBank_clicked();
    void on_btnMyQuestionBankAddNewQuestion_clicked();
    void on_btnAddNewQuestionBack_clicked();
    void on_btnAddNewQuestionAdd_clicked();
    void showQuestionDetails(string questionId);
    void on_btnAddNewQuestionBack_2_clicked();
    void on_btnAddNewQuestionAdd_2_clicked();
    void setUpTestDetails(Test *test);
    void detailsStudentAttempt(StudentAttempt *studentAttempt);
};
#endif // MAINWINDOW_H
