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
        QGroupBox *groupBox = ui->groupBoxDashboard;
        groupBox->show();
        groupBox = ui->groupBoxAddNewTest;
        groupBox->hide();
    }
    else if (managerStudent.login(username, password, logged))
    {
        QMessageBox::information(this, "Login", "Login successful!");
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
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if (choice == "Student" && managerStudent.registerStudent(fullname, username, password))
    {
        QMessageBox::information(this, "Register", "Register successful!");
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::warning(this, "Register", "Form is incorrect. Please try again!");
    }
}

void MainWindow::setUpTeacherDashboard()
{
    QLabel *label = ui->label_4;
    label->setText(QString::number(managerTest.countIncomingTest(logged.getId())));
    label->setAlignment(Qt::AlignCenter);
    label = ui->label_5;
    label->setText(QString::number(managerTest.countRunningTest(logged.getId())));
    label->setAlignment(Qt::AlignCenter);
    label = ui->label_6;
    label->setText(QString::number(managerTest.countCompletedTest(logged.getId())));
    label->setAlignment(Qt::AlignCenter);
    label = ui->lbTeacherName;
    label->setText(QString::fromStdString(logged.getFullname()));
    label->setAlignment(Qt::AlignCenter);
    label = ui->lbWarning;
    label->hide();
    QTableWidget *table = ui->tbTeacherDashboard;
    qDebug() << "Number of tests:" << managerTest.getTestCount();

    int count = 0;
    Test *teacherTests = managerTest.getTestByTeacherId(logged.getId(), count);

    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"ID", "Teacher ID", "Title", "Total Question", "Duration", "Starts At", "Ends At"});
    table->setColumnWidth(0, 50);  // ID
    table->setColumnWidth(1, 70);  // Teacher ID
    table->setColumnWidth(2, 150); // Title
    table->setColumnWidth(3, 100); // Total Question
    table->setColumnWidth(4, 70);  // Duration
    table->setColumnWidth(5, 180); // Starts At
    table->setColumnWidth(6, 180); // Ends At
    table->setRowCount(count);
    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
    if (count == 0)
    {
        qDebug() << "No tests available in testManager.";
        label->show();
        return;
    }
    for (int i = 0; i < count; i++)
    {
        Test *test = teacherTests + i;
        if (test == nullptr)
        {
            qDebug() << "Test not found for ID:" << (i);
            continue;
        }

        table->setItem(i, 0, new QTableWidgetItem(QString::number(test->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(test->getTeacherId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(test->getTitle())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(test->getTotalQuestion())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(test->getDuration())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(test->getStartsAt())));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(test->getEndsAt())));
        for (int j = 0; j < 7; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
            table->item(i, j)->setBackground(test->getStatus() == 0 ? QBrush(QColor("#ffc107")) : test->getStatus() == 1 ? QBrush(QColor("#17a2b8"))
                                                                                                                         : QBrush(QColor("#28a745")));
        }
    }
    table->show();
}

void MainWindow::on_btnLogoutTeacherDashboard_clicked()
{
    logged = currentUser(0, "", "", "", "");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnDashboard_clicked()
{
    QGroupBox *grBox = ui->groupBoxDashboard;
    grBox->show();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
}

void MainWindow::on_btnAddNewTest_clicked()
{
    QGroupBox *grBox = ui->groupBoxAddNewTest;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
}

void MainWindow::on_btnAddNewTextNext_clicked()
{
    QString qTitle = ui->txtAddNewTestNameOfTest->text();
    QString qTotalQuestion = ui->txtAddNewTestTotalQuestion->text();
    QString qDuration = ui->txtAddNewTestDuaration->text();
    QString qPassword = ui->txtAddNewTestPassword->text();
    QDateTime qStartsAt = ui->dtStartAt->dateTime();
    QDateTime qEndsAt = ui->dtEndAt->dateTime();
    QString qstartAt = qStartsAt.toString("hh:mm:ss dd/MM/yyyy");
    QString qEndAt = qEndsAt.toString("hh:mm:ss dd/MM/yyyy");

    int totalQuestion = qTotalQuestion.toInt();
    int duration = qDuration.toInt();
    string title = qTitle.toStdString();
    string startsAt = qstartAt.toStdString();
    string endsAt = qEndAt.toStdString();
    string password = qPassword.toStdString();

    if (managerTest.createTest(logged.getId(), title, totalQuestion, password, duration, startsAt, endsAt))
    {
        QMessageBox::information(this, "Add New Test", "Add new test successful!");
        on_btnDashboard_clicked();
        setUpTeacherDashboard();
    }
    else
    {
        QMessageBox::warning(this, "Add New Test", "Form is incorrect. Please try again!");
    }
}
