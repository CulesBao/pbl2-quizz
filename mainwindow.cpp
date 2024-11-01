#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./src/student/student.h"
#include "./src/teacher/teacher.h"
#include "./src/currentUser/currentUser.h"
#include <QMessageBox>
using namespace std;

currentUser logged(1, "", "", "", "");
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::login);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::login()
{
    QString qUsername = ui->txtUsername->text();
    QString qPassword = ui->txtPassword->text();
    std::string username = qUsername.toStdString();
    std::string password = qPassword.toStdString();

    // if (managerTeacher.login(username, password) or managerStudent.login(username, password)) {
    //     QMessageBox::information(this, "Login", "Login successful!");
    //     ui->stackedWidget->setCurrentIndex(1);
    // } else {
    //     QMessageBox::warning(this, "Login", "Username or password incorrect. Please try again!");
    // }
    if (managerTeacher.login(username, password, logged))
    {
        QMessageBox::information(this, "Login", "Login successful!");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (managerStudent.login(username, password, logged))
    {
        QMessageBox::information(this, "Login", "Login successful!");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::warning(this, "Login", "Username or password incorrect. Please try again!");
    }
}

void MainWindow::on_btnRegister_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnRegister_2_clicked()
{
    QString qFullname = ui->txtFullname->text();
    QString qUsername = ui->txtUsername_2->text();
    QString qPassword = ui->txtPassword_2->text();
    QString qChoice = ui->cmbChoice->currentText();

    string fullname = qFullname.toStdString();
    string username = qUsername.toStdString();
    string password = qPassword.toStdString();
    string choice = qChoice.toStdString();

    if (choice == "Teacher" && managerTeacher.registerTeacher(fullname, username, password))
    {
        QMessageBox::information(this, "Register", "Register successful!");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (choice == "Student" && managerStudent.registerStudent(fullname, username, password))
    {
        QMessageBox::information(this, "Register", "Register successful!");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::warning(this, "Register", "Form is incorrect. Please try again!");
    }
}

void MainWindow::on_btnLogin_clicked()
{
    cout << 0;
}

void MainWindow::on_btnLogoutTeacherDashboard_clicked()
{
    logged = currentUser(0, "", "", "", "");
    ui->stackedWidget->setCurrentIndex(0);
}
