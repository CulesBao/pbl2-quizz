#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./src/student/student.h"
#include "./src/teacher/teacher.h"
#include "./src/currentUser/currentUser.h"
#include "./src/testInfo/testInfo.h"
#include <QMessageBox>
using namespace std;
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

    if (managerTeacher.login(username, password, logged))
    {
        QMessageBox::information(this, "Login", "Login successful!");
        ui->stackedWidget->setCurrentIndex(1);
        setUpTeacherDashboard();
    }
    else if (managerStudent.login(username, password, logged))
    {
        QMessageBox::information(this, "Login", "Login successful!");
        qDebug() << "Welcome, " << QString::fromStdString(logged.getFullname());
        ui->stackedWidget->setCurrentIndex(1);
        setUpTeacherDashboard();
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

void MainWindow::setUpTeacherDashboard()
{
    QLabel *label = ui->lbTeacherName;
    label->setText(QString::fromStdString(logged.getFullname()));
    QTableWidget *table = ui->tbTeacherDashboard;
    qDebug() << "Number of tests:" << managerTest.getTestCount();
    if (managerTest.getTestCount() == 0)
    {
        qDebug() << "No tests available in testManager.";
        return;
    }

    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"ID", "Teacher ID", "Title", "Total Question", "Duration", "Starts At", "Ends At", "Status"});
    table->setRowCount(managerTest.getTestCount());
    for (int i = 0; i < managerTest.getTestCount(); i++)
    {
        Test *test = managerTest.getTestById(i + 1);
        if (test == nullptr)
        {
            qDebug() << "Test not found for ID:" << (i + 1);
            continue;
        }

        table->setItem(i, 0, new QTableWidgetItem(QString::number(test->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(test->getTeacherId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(test->getTitle())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(test->getTotalQuestion())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(test->getDuration())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(test->getStartsAt())));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(test->getEndsAt())));
        table->setItem(i, 7, new QTableWidgetItem(test->getStatus() == 0 ? "Incoming" : test->getStatus() == 1 ? "Running"
                                                                                                               : "Finished"));
    }
    table->show();
}

void MainWindow::on_btnLogin_clicked()
{
}

void MainWindow::on_btnLogoutTeacherDashboard_clicked()
{
    logged = currentUser(0, "", "", "", "");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnDashboard_clicked()
{
}
