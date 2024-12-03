#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./src/student/student.h"
#include "./src/teacher/teacher.h"
#include "./src/currentUser/currentUser.h"
#include "./src/testInfo/testInfo.h"
#include "./src/testQuestionSelection/testQuestionSelection.h"
#include "./src/questionBank/question.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include "studentform.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtPassword->setEchoMode(QLineEdit::Password);

    connect(ui->checkBoxShowHide, &QCheckBox::stateChanged, this, [=](int state)
            {
        if (state == Qt::Checked) {
            ui->txtPassword->setEchoMode(QLineEdit::Normal); // Hiện password
        } else {
            ui->txtPassword->setEchoMode(QLineEdit::Password); // Ẩn password
        } });
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
        ui->stackedWidget->setCurrentIndex(1);
        setUpTeacherDashboard();
        QGroupBox *groupBox = ui->groupBoxDashboard;
        groupBox->show();
        groupBox = ui->groupBoxAddNewTest;
        groupBox->hide();
        groupBox = ui->groupBoxMyQuestionBank;
        groupBox->hide();
        groupBox = ui->groupBoxEditProfile;
        groupBox->hide();
        groupBox = ui->groupBoxAddNewQuestion;
        groupBox->hide();
        groupBox = ui->groupBoxDetailsQuestion;
        groupBox->hide();
        groupBox = ui->groupBoxTestDetails;
        groupBox->hide();
    }
    else if (managerStudent.login(username, password, logged))
    {
        this->hide();
        StudentForm *secWindow = new StudentForm();
        secWindow->show();
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
    table->clearContents();
    table->setRowCount(0);
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
        label->show();
        return;
    }
    for (int i = 0; i < count; i++)
    {
        Test *test = teacherTests + i;
        if (test == nullptr)
        {
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
                { setUpTestDetails(test); });

        connect(btnDelete, &QPushButton::clicked, this, [this, i, test]()
                {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete Confirmation",
                                          "Are you sure you want to delete test: " + QString::fromStdString(test->getId()) + "?",
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
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
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
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
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();

    QTableWidget *table = ui->tbSetNumberQuestion;
    table->clearContents();
    table->setRowCount(0);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Chapter", "Number of Questions", "You have"});
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 140);
    table->setColumnWidth(2, 70);

    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");

    QLineEdit *lineEdit = ui->txtAddNewTestNameOfTest;
    lineEdit->clear();
    lineEdit = ui->txtAddNewTestDuaration;
    lineEdit->clear();
    lineEdit = ui->txtAddNewTestPassword;
    lineEdit->clear();
    QDateTimeEdit *dtStartAt = ui->dtStartAt;
    dtStartAt->setDateTime(QDateTime::currentDateTime());
    QDateTimeEdit *dtEndAt = ui->dtEndAt;
    dtEndAt->setDateTime(QDateTime::currentDateTime());

    for (int i = 0; i < chapterManager.getChapterCount(); i++)
    {
        Chapter *chapter = chapterManager.getChapterById(i);
        table->insertRow(i);
        table->setRowHeight(i, 70);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(chapter->getName())));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(questionBankManager.getNumberOfQuestion(logged.getId(), chapter->getId()))));
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
        bool isFormCorrect = true;
        for (int i = 0; i < ui->tbSetNumberQuestion->rowCount(); i++)
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(ui->tbSetNumberQuestion->cellWidget(i, 1));
            int count = lineEdit->text().toInt();
            if (count > 0)
            {
                Chapter *chapter = chapterManager.getChapterById(i);
                int totalQuestion = questionBankManager.getNumberOfQuestion(logged.getId(), chapter->getId());
                if (count > totalQuestion)
                {
                    isFormCorrect = false;
                    QMessageBox::warning(this, "Add New Test", QString::fromStdString(chapter->getName()) + " has only " + QString::number(totalQuestion) + " questions. Please try again!");
                    return;
                }
            }
        }
        if (managerTest.createTest(logged.getId(), title, questionCount, password, duration, startsAt, endsAt) && isFormCorrect)
        {
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
            QMessageBox::information(this, "Add New Test", "Add new test successful!");
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
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
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
        logged.setFullname(fullname);
        logged.setPassword(password);
    }
    else
    {
        QMessageBox::warning(this, "Edit Profile", "Form is incorrect. Please try again!");
    }
}

void MainWindow::on_btnMyQuestionBank_clicked()
{
    QLabel *label = ui->lbWarning_2;
    label->hide();
    QGroupBox *grBox = ui->groupBoxMyQuestionBank;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();

    QTableWidget *table = ui->tbMyQuestionBank;
    table->clearContents();
    table->setRowCount(0);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Chapter Id", "Question", "Details", "Delete"});
    table->setColumnWidth(0, 70);  // ID
    table->setColumnWidth(1, 70);  // Chapter
    table->setColumnWidth(2, 250); // Question
    table->setColumnWidth(3, 60);  // Details
    table->setColumnWidth(4, 60);  // Delete

    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");

    int count = 0;
    Question *questions = questionBankManager.getQuestionByTeacherId(logged.getId(), count);
    if (count == 0)
    {
        qDebug() << "No question found for teacher ID:" << QString::fromStdString(logged.getId());
        label->show();
        return;
    }
    for (int i = 0; i < count; i++)
    {
        Question *question = questions + i;
        if (question == nullptr)
        {
            qDebug() << "Question not found for ID:" << (i);
            continue;
        }

        table->insertRow(i);
        table->setRowHeight(i, 70);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(question->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(question->getChapterId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(question->getQuestionText())));
        for (int j = 0; j < 2; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
        QPushButton *btnDetails = new QPushButton("Details");
        table->setCellWidget(i, 3, btnDetails);
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
            "min-height: 15px;"
            "max-height: 15px;"
            "text-align: center;"
            "cursor: pointer;"
            "align-items: center;"
            "outline: none;");
        btnDetails->setCursor(Qt::PointingHandCursor);

        QPushButton *btnDelete = new QPushButton("Delete");
        table->setCellWidget(i, 4, btnDelete);
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
            "min-height: 15px;"
            "max-height: 15px;"
            "text-align: center;"
            "cursor: pointer;"
            "align-items: center;"
            "outline: none;");
        btnDelete->setCursor(Qt::PointingHandCursor);
        connect(btnDetails, &QPushButton::clicked, this, [this, question]()
                { showQuestionDetails(question->getId()); });
        connect(btnDelete, &QPushButton::clicked, this, [this, i, question]()
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Delete Confirmation",
                                                  "Are you sure you want to delete question: " + QString::fromStdString(question->getId()) + "?",
                                                  QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        if (!questionBankManager.deleteQuestion(question->getId()))
                            QMessageBox::warning(this, "Delete Question", "Failed to delete question. Please try again!");
                        else
                            ui->tbMyQuestionBank->removeRow(i);
                    }; });
    }
}

void MainWindow::on_btnMyQuestionBankAddNewQuestion_clicked()
{
    QGroupBox *grBox = ui->groupBoxAddNewQuestion;
    grBox->show();
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();

    QComboBox *cmbChapter = ui->cmbChapter;
    cmbChapter->clear();
    for (int i = 0; i < chapterManager.getChapterCount(); i++)
    {
        Chapter *chapter = chapterManager.getChapterById(i);
        cmbChapter->addItem(QString::fromStdString(chapter->getName()));
        cmbChapter->setStyleSheet("color: black;");
    }

    QPlainTextEdit *txtQuestion = ui->plainTextEditQuestionTest;
    txtQuestion->clear();

    QRadioButton *radioButton = ui->radioButton;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_2;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_3;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_4;
    radioButton->setChecked(false);

    QLineEdit *lineEdit = ui->lineEdit;
    lineEdit->clear();
    lineEdit = ui->lineEdit_2;
    lineEdit->clear();
    lineEdit = ui->lineEdit_3;
    lineEdit->clear();
    lineEdit = ui->lineEdit_4;
    lineEdit->clear();
}

void MainWindow::on_btnAddNewQuestionBack_clicked()
{
    QGroupBox *grBox = ui->groupBoxMyQuestionBank;
    grBox->show();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();
}

void MainWindow::on_btnAddNewQuestionAdd_clicked()
{
    QString qChapter = ui->cmbChapter->currentText();

    string chapter = qChapter.toStdString();
    string question = ui->plainTextEditQuestionTest->toPlainText().toStdString();
    string optionsArr[4];

    QLineEdit *lineEdit = ui->lineEdit;
    optionsArr[0] = lineEdit->text().toStdString();
    lineEdit = ui->lineEdit_2;
    optionsArr[1] = lineEdit->text().toStdString();
    lineEdit = ui->lineEdit_3;
    optionsArr[2] = lineEdit->text().toStdString();
    lineEdit = ui->lineEdit_4;
    optionsArr[3] = lineEdit->text().toStdString();
    int numberOfOptions = 4;

    int correctAnswer = 0;
    if (ui->radioButton_2->isChecked())
    {
        correctAnswer = 2;
    }
    else if (ui->radioButton_3->isChecked())
    {
        correctAnswer = 1;
    }
    else if (ui->radioButton_4->isChecked())
    {
        correctAnswer = 4;
    }
    else if (ui->radioButton->isChecked())
    {
        correctAnswer = 3;
    }

    if (correctAnswer == 0)
    {
        QMessageBox::warning(this, "Add New Question", "Please select correct answer. Please try again!");
        return;
    }

    string chapterId = chapterManager.getChapterIdByName(chapter);

    if (questionBankManager.addQuestion(logged.getId(), chapterId, question, numberOfOptions, optionsArr, correctAnswer))
    {
        QMessageBox::information(this, "Add New Question", "Add new question successful!");
        on_btnMyQuestionBank_clicked();
    }
    else
    {
        QMessageBox::warning(this, "Add New Question", "Form is incorrect. Please try again!");
    }
}
void MainWindow::showQuestionDetails(string questionId)
{
    QGroupBox *grBox = ui->groupBoxDetailsQuestion;
    grBox->show();
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();

    Question *question = questionBankManager.getQuestionById(questionId);
    if (question == nullptr)
    {
        QMessageBox::warning(this, "Show Question Details", "Question not found. Please try again!");
        return;
    }

    QComboBox *cmbChapter = ui->cmbChapter_2;
    cmbChapter->clear();
    for (int i = 0; i < chapterManager.getChapterCount(); i++)
    {
        Chapter *chapter = chapterManager.getChapterById(i);
        cmbChapter->addItem(QString::fromStdString(chapter->getName()));
        cmbChapter->setStyleSheet("color: black;");

        if (chapter->getId() == question->getChapterId())
        {
            cmbChapter->setCurrentIndex(i);
        }
    }
    QPlainTextEdit *qQuestionTest = ui->plainTextEditQuestionTest_2;
    qQuestionTest->clear();
    qQuestionTest->setPlainText(QString::fromStdString(question->getQuestionText()));

    QRadioButton *radioButton = ui->radioButton;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_2;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_3;
    radioButton->setChecked(false);
    radioButton = ui->radioButton_4;
    radioButton->setChecked(false);

    switch (question->getCorrectAnswerId())
    {
    case 1:
        radioButton = ui->radioButton_5;
        radioButton->setChecked(true);
        break;
    case 2:
        radioButton = ui->radioButton_6;
        radioButton->setChecked(true);
        break;
    case 3:
        radioButton = ui->radioButton_7;
        radioButton->setChecked(true);
        break;
    case 4:
        radioButton = ui->radioButton_8;
        radioButton->setChecked(true);
        break;
    default:
        break;
    }

    QLineEdit *lineEdit = ui->lineEdit_5;
    lineEdit->setText(QString::fromStdString(question->getOption(0)));
    lineEdit = ui->lineEdit_6;
    lineEdit->setText(QString::fromStdString(question->getOption(1)));
    lineEdit = ui->lineEdit_7;
    lineEdit->setText(QString::fromStdString(question->getOption(2)));
    lineEdit = ui->lineEdit_8;
    lineEdit->setText(QString::fromStdString(question->getOption(3)));

    connect(ui->btnAddNewQuestionAdd_2, &QPushButton::clicked, this, [this, question, qQuestionTest]()
            {
                QString qChapter = ui->cmbChapter_2->currentText();
                QPlainTextEdit *txtQuestion = ui->plainTextEditQuestionTest_2;

                string chapter = qChapter.toStdString();
                string questionText = qQuestionTest->toPlainText().toStdString();
                int numberOfOptions = 4;
                int correctAnswer = 0;
                if (ui->radioButton_5->isChecked())
                {
                    correctAnswer = 1;
                }
                else if (ui->radioButton_6->isChecked())
                {
                    correctAnswer = 2;
                }
                else if (ui->radioButton_7->isChecked())
                {
                    correctAnswer = 3;
                }
                else if (ui->radioButton_8->isChecked())
                {
                    correctAnswer = 4;
                }
                string optionsArr[numberOfOptions];
                QLineEdit *lineEdit = ui->lineEdit_5;
                optionsArr[0] = lineEdit->text().toStdString();
                lineEdit = ui->lineEdit_6;
                optionsArr[1] = lineEdit->text().toStdString();
                lineEdit = ui->lineEdit_7;
                optionsArr[2] = lineEdit->text().toStdString();
                lineEdit = ui->lineEdit_8;
                optionsArr[3] = lineEdit->text().toStdString();

                string chapterId = chapterManager.getChapterIdByName(chapter);

                if (questionBankManager.updateQuestion(question->getId(), logged.getId(), chapterId, questionText, numberOfOptions, optionsArr, correctAnswer))
                {
                    QMessageBox::information(this, "Edit Question", "Edit question successful!");
                    on_btnMyQuestionBank_clicked();
                }
                else
                {
                    QMessageBox::warning(this, "Edit Question", "Form is incorrect. Please try again!");
                } });
}

void MainWindow::on_btnAddNewQuestionBack_2_clicked()
{
    QGroupBox *grBox = ui->groupBoxMyQuestionBank;
    grBox->show();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();
    grBox = ui->groupBoxTestDetails;
    grBox->hide();
}

void MainWindow::on_btnAddNewQuestionAdd_2_clicked()
{
}

void MainWindow::on_btnGenerateFile_clicked()
{
}
void MainWindow::setUpTestDetails(Test *test)
{
    QGroupBox *grBox = ui->groupBoxTestDetails;
    grBox->show();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxDetailsQuestion;
    grBox->hide();

    QTableWidget *table = ui->tbTestDetails;
    table->clearContents();
    table->setColumnCount(9);
    table->setRowCount(0);

    table->setHorizontalHeaderLabels({"Student ID", "Fullname", "Starts At", "Ends At", "Number of Question", "Correct Answer", "Score", "Details", "Delete"});
    table->setColumnWidth(0, 100); // Student ID
    table->setColumnWidth(1, 200); // Fullname
    table->setColumnWidth(2, 150); // Starts At
    table->setColumnWidth(3, 150); // Ends At
    table->setColumnWidth(4, 150); // Number of Question
    table->setColumnWidth(5, 150); // Correct Answer
    table->setColumnWidth(6, 60);  // Score
    table->setColumnWidth(7, 60);  // Details
    table->setColumnWidth(8, 60);  // Delete

    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");

    int count = 0;
    StudentAttempt *studentAttempts = studentAttemptManager.getAttemptByTestId(test->getId(), count);

    if (count == 0)
    {
        QMessageBox::warning(this, "Test Details", "No student attempt this test. Please try again!");
        return;
    }
    table->setStyleSheet("QTableWidget::item { border: 1px solid #888; color: black; }");
    for (int i = 0; i < count; i++)
    {
        StudentAttempt *studentAttempt = studentAttempts + i;
        if (studentAttempt == nullptr)
        {
            QMessageBox::warning(this, "Test Details", "Student attempt not found. Please try again!");
            continue;
        }

        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(studentAttempt->getStudentId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(managerStudent.getNameById(studentAttempt->getStudentId()))));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(studentAttempt->getStartsAt())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(studentAttempt->getFinishedAt())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(test->getTotalQuestion())));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(studentAttempt->getCorrectAnswer())));
        table->setItem(i, 6, new QTableWidgetItem(QString::number((studentAttempt->getCorrectAnswer() * 10.0) / test->getTotalQuestion(), 'f', 2)));
        for (int j = 0; j < 7; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
        QPushButton *btnDetails = new QPushButton("Details");
        table->setCellWidget(i, 7, btnDetails);
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
            "min-height: 15px;"
            "max-height: 15px;"
            "text-align: center;"
            "cursor: pointer;"
            "outline: none;");
        connect(btnDetails, &QPushButton::clicked, this, [this, studentAttempt]()
                { detailsStudentAttempt(studentAttempt); });

        QPushButton *btnDelete = new QPushButton("Delete");
        table->setCellWidget(i, 8, btnDelete);
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
            "min-height: 15px;"
            "max-height: 15px;"
            "text-align: center;"
            "cursor: pointer;"
            "outline: none;");
        connect(btnDelete, &QPushButton::clicked, this, [this, studentAttempt, i]()
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Delete Confirmation",
                                                  "Are you sure you want to delete student attempt: " + QString::fromStdString(studentAttempt->getId()) + "?",
                                                  QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        if (!studentAttemptManager.deleteAttempt(studentAttempt->getId()))
                            QMessageBox::warning(this, "Delete Attempt", "Failed to delete attempt. Please try again!");
                        else
                            ui->tbTestDetails->removeRow(i);
                    }; });
    }
    disconnect(ui->btnGenerateFile, &QPushButton::clicked, nullptr, nullptr);
    connect(ui->btnGenerateFile, &QPushButton::clicked, this, [this, test]()
            { 
                qDebug() << test->getId();
                QString filePath = QFileDialog::getSaveFileName(
                        nullptr,                           // Parent widget (nullptr nếu không có)
                        "Chọn vị trí lưu file",            // Tiêu đề của hộp thoại
                        QDir::homePath(),                  // Thư mục mặc định (thư mục Home của người dùng)
                        "CSV Files (*.csv);;All Files (*)" // Bộ lọc định dạng file
                    );

                    if (!filePath.isEmpty())
                    {
                        QFile file(filePath);
                        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            QTextStream out(&file);
                            // Write data
                            out << "TEST DETAILS" << Qt::endl;
                            out << "Test ID: " << QString::fromStdString(test->getId()) << Qt::endl;
                            out << "Title: " << QString::fromStdString(test->getTitle()) << Qt::endl;
                            out << "Number of Questions: " << QString::number(test->getTotalQuestion()) << Qt::endl;
                            out << "Password: " << QString::fromStdString(test->getPassword()) << Qt::endl;
                            out << "Duration: " << QString::number(test->getDuration()) << " minutes" << Qt::endl;
                            out << "Starts At: " << QString::fromStdString(test->getStartsAt()) << Qt::endl;
                            out << "Ends At: " << QString::fromStdString(test->getEndsAt()) << Qt::endl
                                << Qt::endl;

                            out << "Student ID, Fullname, Starts At, Ends At, Number of Question, Correct Answer, Score" << Qt::endl;
                            int count = 0;
                            StudentAttempt *studentAttempts = studentAttemptManager.getAttemptByTestId(test->getId(), count);
                            for (int i = 0; i < count; i++)
                            {
                                StudentAttempt *studentAttempt = studentAttempts + i;
                                if (studentAttempt == nullptr)
                                {
                                    qDebug() << "Student attempt not found. Please try again!";
                                    continue;
                                }
                                out << QString::fromStdString(studentAttempt->getStudentId()) << ", "
                                    << QString::fromStdString(managerStudent.getNameById(studentAttempt->getStudentId())) << ", "
                                    << QString::fromStdString(studentAttempt->getStartsAt()) << ", "
                                    << QString::fromStdString(studentAttempt->getFinishedAt()) << ", "
                                    << QString::number(test->getTotalQuestion()) << ", "
                                    << QString::number(studentAttempt->getCorrectAnswer()) << ", "
                                    << QString::number((studentAttempt->getCorrectAnswer() * 10.0) / test->getTotalQuestion(), 'f', 2) << Qt::endl;
                            }
                            file.close();
                            QMessageBox::information(this, "File Saved", "File saved successfully!");
                        }
                        else
                        {
                            QMessageBox::warning(this, "File Error", "Unable to open file for writing.");
                        }
                    } });
}

void MainWindow::btnGenerateFile_clicked(Test *test)
{
    QString filePath = QFileDialog::getSaveFileName(
        nullptr,                           // Parent widget (nullptr nếu không có)
        "Chọn vị trí lưu file",            // Tiêu đề của hộp thoại
        QDir::homePath(),                  // Thư mục mặc định (thư mục Home của người dùng)
        "CSV Files (*.csv);;All Files (*)" // Bộ lọc định dạng file
    );

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            // Write data
            out << "TEST DETAILS" << Qt::endl;
            out << "Test ID: " << QString::fromStdString(test->getId()) << Qt::endl;
            out << "Title: " << QString::fromStdString(test->getTitle()) << Qt::endl;
            out << "Number of Questions: " << QString::number(test->getTotalQuestion()) << Qt::endl;
            out << "Password: " << QString::fromStdString(test->getPassword()) << Qt::endl;
            out << "Duration: " << QString::number(test->getDuration()) << " minutes" << Qt::endl;
            out << "Starts At: " << QString::fromStdString(test->getStartsAt()) << Qt::endl;
            out << "Ends At: " << QString::fromStdString(test->getEndsAt()) << Qt::endl
                << Qt::endl;

            out << "Student ID, Fullname, Starts At, Ends At, Number of Question, Correct Answer, Score" << Qt::endl;
            int count = 0;
            StudentAttempt *studentAttempts = studentAttemptManager.getAttemptByTestId(test->getId(), count);
            for (int i = 0; i < count; i++)
            {
                StudentAttempt *studentAttempt = studentAttempts + i;
                if (studentAttempt == nullptr)
                {
                    qDebug() << "Student attempt not found. Please try again!";
                    continue;
                }
                out << QString::fromStdString(studentAttempt->getStudentId()) << ", "
                    << QString::fromStdString(managerStudent.getNameById(studentAttempt->getStudentId())) << ", "
                    << QString::fromStdString(studentAttempt->getStartsAt()) << ", "
                    << QString::fromStdString(studentAttempt->getFinishedAt()) << ", "
                    << QString::number(test->getTotalQuestion()) << ", "
                    << QString::number(studentAttempt->getCorrectAnswer()) << ", "
                    << QString::number((studentAttempt->getCorrectAnswer() * 10.0) / test->getTotalQuestion(), 'f', 2) << Qt::endl;
            }
            file.close();
            QMessageBox::information(this, "File Saved", "File saved successfully!");
        }
        else
        {
            QMessageBox::warning(this, "File Error", "Unable to open file for writing.");
        }
    }
}

void MainWindow::detailsStudentAttempt(StudentAttempt *studentAttempt)
{
    QGroupBox *grBox = ui->groupBoxTestDetails;
    grBox->hide();
    grBox = ui->groupBoxAddNewTest;
    grBox->hide();
    grBox = ui->groupBoxAddNewQuestion;
    grBox->hide();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxMyQuestionBank;
    grBox->hide();
    grBox = ui->groupBoxDetailStudentAttempt;
    grBox->show();

    QTableWidget *table = ui->tbStudentAttempt;
    table->clearContents();
    table->setColumnCount(4);
    table->setRowCount(0);

    table->setHorizontalHeaderLabels({"Question ID", "Question", "Your Answer", "Correct Answer"});
    table->setStyleSheet(
        "QTableWidget::item:selected {"
        "    background-color: transparent;" // Giữ màu nền không thay đổi
        "    color: inherit;"                // Giữ nguyên màu chữ
        "    border: none;"                  // Không viền
        "}");

    table->setColumnWidth(0, 100); // Question ID
    table->setColumnWidth(1, 300); // Question
    table->setColumnWidth(2, 150); // Your Answer
    table->setColumnWidth(3, 150); // Correct Answer

    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");

    for (int i = 0; i < studentAttempt->getTotalQuestions(); i++)
    {
        Question *tmpQuestion = questionBankManager.getQuestionById(studentAttempt->getQuestionId(i));

        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(tmpQuestion->getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tmpQuestion->getQuestionText())));
        if (studentAttempt->getStudentAnswer(i) == 0)
        {
            table->setItem(i, 2, new QTableWidgetItem("x"));
        }
        else
        {
            table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tmpQuestion->getOption(studentAttempt->getStudentAnswer(i) - 1))));
        }
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tmpQuestion->getOption(tmpQuestion->getCorrectAnswerId() - 1))));
        for (int j = 0; j < 4; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
        if (studentAttempt->getStudentAnswer(i) == tmpQuestion->getCorrectAnswerId())
        {
            for (int j = 0; j < 4; j++)
            {
                table->item(i, j)->setBackground(QBrush(QColor("#28a745"))); // Green
            }
        }
        else
        {
            for (int j = 0; j < 4; j++)
            {
                table->item(i, j)->setBackground(QBrush(QColor("#dc3545"))); // Red
            }
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing
    table->setSelectionMode(QAbstractItemView::NoSelection);   // Prevent selection

    disconnect(ui->btnGenerateFile_2, &QPushButton::clicked, nullptr, nullptr);
    connect(ui->btnGenerateFile_2, &QPushButton::clicked, this, [this, studentAttempt]()
            { 
                QString filePath = QFileDialog::getSaveFileName(
                        nullptr,                           // Parent widget (nullptr nếu không có)
                        "Chọn vị trí lưu file",            // Tiêu đề của hộp thoại
                        QDir::homePath(),                  // Thư mục mặc định (thư mục Home của người dùng)
                        "CSV Files (*.csv);;All Files (*)" // Bộ lọc định dạng file
                    );

                    if (!filePath.isEmpty())
                    {
                        QFile file(filePath);
                        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            QTextStream out(&file);
                            // Write data
                            out << "STUDENT ATTEMPT DETAILS" << Qt::endl;
                            out << "Student ID: " << QString::fromStdString(studentAttempt->getStudentId()) << Qt::endl;
                            out << "Test ID: " << QString::fromStdString(studentAttempt->getTestId()) << Qt::endl;
                            out << "Starts At: " << QString::fromStdString(studentAttempt->getStartsAt()) << Qt::endl;
                            out << "Ends At: " << QString::fromStdString(studentAttempt->getFinishedAt()) << Qt::endl;
                            out << "Number of Questions: " << QString::number(studentAttempt->getTotalQuestions()) << Qt::endl;
                            out << "Correct Answer: " << QString::number(studentAttempt->getCorrectAnswer()) << Qt::endl
                                << Qt::endl;

                            out << "Question ID, Question, Your Answer, Correct Answer" << Qt::endl;
                            for (int i = 0; i < studentAttempt->getTotalQuestions(); i++)
                            {
                                Question *tmpQuestion = questionBankManager.getQuestionById(studentAttempt->getQuestionId(i));
                                out << QString::fromStdString(tmpQuestion->getId()) << ", "
                                    << QString::fromStdString(tmpQuestion->getQuestionText()) << ", ";
                                if (studentAttempt->getStudentAnswer(i) == 0)
                                {
                                    out << "x, ";
                                }
                                else
                                {
                                    out << QString::fromStdString(tmpQuestion->getOption(studentAttempt->getStudentAnswer(i) - 1)) << ", ";
                                }
                                out << QString::fromStdString(tmpQuestion->getOption(tmpQuestion->getCorrectAnswerId() - 1)) << Qt::endl;
                            }
                            file.close();
                            QMessageBox::information(this, "File Saved", "File saved successfully!");
                        }
                        else
                        {
                            QMessageBox::warning(this, "File Error", "Unable to open file for writing.");
                        }} });
}
