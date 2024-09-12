#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "teacher.h"
#include <QMessageBox>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    bool isOke = manager.login(username, password);
    if (isOke) {
        QMessageBox::information(this, "Login", "Login successful!");
        ui->stackedWidget->setCurrentIndex(1);
    } else {
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
    QString qFullname = ui -> txtFullname -> text();
    QString qUsername = ui -> txtUsername_2 -> text();
    QString qPassword = ui -> txtPassword_2 -> text();
    QString qChoice = ui -> cmbChoice -> currentText();

    string fullname = qFullname.toStdString();
    string username = qUsername.toStdString();
    string password = qPassword.toStdString();
    string choice = qChoice.toStdString();

    if (choice == "Teacher" && manager.registerTeacher(fullname, username, password)){
        QMessageBox::information(this, "Register", "Register successful!");
        ui->stackedWidget->setCurrentIndex(1);
    }
    else{
        QMessageBox::warning(this, "Register", "Form is incorrect. Please try again!");
    }
}

