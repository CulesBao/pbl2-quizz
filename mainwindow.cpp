#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./src/student/student.h"
#include "./src/teacher/teacher.h"
#include "./src/currentUser/currentUser.h"
#include "./src/testInfo/testInfo.h"
#include "./src/testQuestionSelection/testQuestionSelection.h"
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
    int count = 0;
    Test *teacherTests = managerTest.getTestByTeacherId(logged.getId(), count);
    table->setColumnCount(10);
    table->setHorizontalHeaderLabels({"ID", "Teacher ID", "Title", "Total Question", "Duration", "Password", "Starts At", "Ends At", "Detail", "Delete"});
    table->setColumnWidth(0, 70);  // ID
    table->setColumnWidth(1, 80);  // Teacher ID
    table->setColumnWidth(2, 120); // Title
    table->setColumnWidth(3, 90);  // Total Question
    table->setColumnWidth(4, 60);  // Duration
    table->setColumnWidth(5, 80);  // Password
    table->setColumnWidth(8, 160); // Starts At
    table->setColumnWidth(7, 160); // Ends At
    table->setColumnWidth(8, 60);  // Detail
    table->setColumnWidth(9, 60);  // Delete
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

        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(test->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(test->getTeacherId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(test->getTitle())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(test->getTotalQuestion())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(test->getDuration())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(test->getPassword())));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(test->getStartsAt())));
        table->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(test->getEndsAt())));
        for (int j = 0; j < 8; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
            table->item(i, j)->setBackground(test->getStatus() == 0 ? QBrush(QColor("#ffc107")) : test->getStatus() == 1 ? QBrush(QColor("#17a2b8"))
                                                                                                                         : QBrush(QColor("#28a745")));
        }
        QPushButton *btnDetails = new QPushButton("Details");
        table->setCellWidget(i, 8, btnDetails);
        btnDetails->setStyleSheet(
            "padding: 5px 10px;"
            "font-size: 10px;"
            "border-radius: 5px;"
            "border: 1px solid #ddd;"
            "background-color: #007bff;"
            "color: white;"
            "margin: 2px;"
            "min-width: 30px;"
            "max-width: 30px;"
            "min-height: 7px;"
            "max-height: 7px;"
            "text-align: center;"
            "cursor: pointer;"
            "outline: none;");
        QPushButton *btnDelete = new QPushButton("Delete");
        table->setCellWidget(i, 9, btnDelete);
        btnDelete->setStyleSheet(
            "padding: 5px 10px;"
            "font-size: 10px;"
            "border-radius: 5px;"
            "border: 1px solid #ddd;"
            "background-color: #dc3545;"
            "color: white;"
            "margin: 2px;"
            "min-width: 30px;"
            "max-width: 30px;"
            "min-height: 7px;"
            "max-height: 7px;"
            "text-align: center;"
            "cursor: pointer;"
            "outline: none;");
        connect(btnDetails, &QPushButton::clicked, this, [this, test]()
                {
                    qDebug() << "Show details for Test ID:" << QString::fromStdString(test->getId());
                    // showTestDetails(test);
                });

        connect(btnDelete, &QPushButton::clicked, this, [this, i, test]()
                {
            qDebug() << "Delete Test ID:" << QString::fromStdString(test->getId());
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete Confirmation",
                                          "Are you sure you want to delete test: " + QString::fromStdString(test->getId()) + "?",
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                qDebug() << "Test deleted with ID:" << QString::fromStdString(test->getId());
                if (!managerTest.deleteTestById(test->getId()))
                    QMessageBox::warning(this, "Delete Test", "Failed to delete test. Please try again!");
                else
                    ui->tbTeacherDashboard->removeRow(i);
            }; });
    }
    table->show();
}

void MainWindow::on_btnLogoutTeacherDashboard_clicked()
{
    logged = currentUser("", "", "", "", "");
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
    grBox = ui->groupBoxEditProfile;
    grBox->hide();

    QTableWidget *table = ui->tbSetNumberQuestion;
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Chapter", "Number of Questions"});
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 200);

    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");

    qDebug() << "Number of chapters:" << chapterManager.getChapterCount();
    for (int i = 0; i < chapterManager.getChapterCount(); i++)
    {
        Chapter *chapter = chapterManager.getChapterById(i);
        table->insertRow(i);
        table->setRowHeight(i, 70);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(chapter->getName())));

        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setText("0");
        lineEdit->setAlignment(Qt::AlignCenter);

        table->setCellWidget(i, 1, lineEdit);
        lineEdit->setStyleSheet("border: 1px solid #888; border-radius: 5px; padding: 5px; background-color: #F8F8F8; color: black;");

        connect(lineEdit, &QLineEdit::textChanged, this, [this, lineEdit, chapter]
                {
                    int tmpCount = 0;
                    for (int i = 0; i < ui->tbSetNumberQuestion->rowCount(); i++)
                    {
                        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(ui->tbSetNumberQuestion->cellWidget(i, 1));
                        tmpCount += lineEdit->text().toInt();
                    }
                    ui->txtAddNewTestTotalQuestion->setText(QString::number(tmpCount)); });
    }
    ui->txtAddNewTestTotalQuestion->setReadOnly(true);
}

void MainWindow::on_btnAddNewTextNext_clicked()
{
    QString qTitle = ui->txtAddNewTestNameOfTest->text();
    QString qDuration = ui->txtAddNewTestDuaration->text();
    QString qPassword = ui->txtAddNewTestPassword->text();
    QDateTime qStartsAt = ui->dtStartAt->dateTime();
    QDateTime qEndsAt = ui->dtEndAt->dateTime();
    QString qstartAt = qStartsAt.toString("hh:mm:ss dd/MM/yyyy");
    QString qEndAt = qEndsAt.toString("hh:mm:ss dd/MM/yyyy");

    int duration = qDuration.toInt();
    string title = qTitle.toStdString();
    string startsAt = qstartAt.toStdString();
    string endsAt = qEndAt.toStdString();
    string password = qPassword.toStdString();

    int questionCount = 0;
    for (int i = 0; i < ui->tbSetNumberQuestion->rowCount(); i++)
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(ui->tbSetNumberQuestion->cellWidget(i, 1));
        questionCount += lineEdit->text().toInt();
    }
    if (questionCount == 0)
    {
        QMessageBox::warning(this, "Add New Test", "Total question must be greater than 0. Please try again!");
        return;
    }
    else
    {
        if (managerTest.createTest(logged.getId(), title, questionCount, password, duration, startsAt, endsAt))
        {
            QMessageBox::information(this, "Add New Test", "Add new test successful!");
            for (int i = 0; i < ui->tbSetNumberQuestion->rowCount(); i++)
            {
                QLineEdit *lineEdit = qobject_cast<QLineEdit *>(ui->tbSetNumberQuestion->cellWidget(i, 1));
                int count = lineEdit->text().toInt();
                if (count > 0)
                {
                    Chapter *chapter = chapterManager.getChapterById(i);
                    Test *test = managerTest.getLastTest();
                    if (test != nullptr)
                    {
                        testQuestionSelectionManager.addTestQuestionSelection(test->getId(), chapter->getId(), count);
                    }
                }
            }
            on_btnDashboard_clicked();
            setUpTeacherDashboard();
        }
        else
        {
            QMessageBox::warning(this, "Add New Test", "Form is incorrect. Please try again!");
        }
    }
}

void MainWindow::on_btnEditProfile_clicked()
{
    QGroupBox *grBox = ui->groupBoxEditProfile;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    QLineEdit *txtFullname = ui->txtEditProfileFullname;
    QLineEdit *txtUsername = ui->txtEditProfileUsername;
    QLineEdit *txtPassword = ui->txtEditProfilePassword;
    QLineEdit *txtId = ui->txtEditProfileId;

    txtFullname->setText(QString::fromStdString(logged.getFullname()));
    txtUsername->setText(QString::fromStdString(logged.getUsername()));
    txtPassword->setText(QString::fromStdString(logged.getPassword()));
    txtId->setText(QString::fromStdString(logged.getId()));

    txtUsername->setReadOnly(true);
    txtId->setReadOnly(true);
}

void MainWindow::on_btnEditProfileSubmit_clicked()
{
    QString qFullname = ui->txtEditProfileFullname->text();
    QString qPassword = ui->txtEditProfilePassword->text();

    string fullname = qFullname.toStdString();
    string password = qPassword.toStdString();

    if (managerTeacher.update(logged.getId(), password, fullname))
    {
        QMessageBox::information(this, "Edit Profile", "Edit profile successful!");
        on_btnDashboard_clicked();
        setUpTeacherDashboard();
        QLabel *label = ui->lbTeacherName;
        label->setText(QString::fromStdString(fullname));
    }
    else
    {
        QMessageBox::warning(this, "Edit Profile", "Form is incorrect. Please try again!");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QGroupBox *grBox = ui->groupBoxAddNewTest;
    grBox->show();
    grBox = ui->groupBoxAdvancedSetup;
    grBox->hide();
}

void MainWindow::on_btnAdvancedSetupNext_clicked()
{
}
