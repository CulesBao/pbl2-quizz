#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "teacher.h"
#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui {
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
    void on_pushButton_clicked();
    void on_btnRegister_2_clicked();
    void on_btnBack_clicked();
};
#endif // MAINWINDOW_H
