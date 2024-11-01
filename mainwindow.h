#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "./src/teacher/teacher.h"
#include "./src/student/student.h"

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
private slots:
    void login();
    void on_btnRegister_clicked();
    void on_btnRegister_2_clicked();
    void on_btnBack_clicked();
    void on_btnLogin_clicked();
    void on_btnLogoutTeacherDashboard_clicked();
};
#endif // MAINWINDOW_H
