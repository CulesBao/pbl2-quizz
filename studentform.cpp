#include "studentform.h"
#include "ui_studentform.h"
#include "mainwindow.h"
#include "./src/currentUser/currentUser.h"
#include "./src/testInfo/testInfo.h"
#include "./src/studentAttempt/studentAttempt.h"
#include "./src/questionBank/question.h"
#include "./src/student/student.h"
#include <QDebug>
#include <QInputDialog>
#include <thread>
#include <chrono>
#include <QRadioButton>

StudentForm::StudentForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::StudentForm)
{
    ui->setupUi(this);
    QGroupBox *grBox = ui->groupBoxDashboard;
    grBox->show();
    grBox = ui->leftSidebar;
    grBox->show();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxHistory;
    grBox->hide();

    logged.loadFromFile();
    QLabel *label = ui->lbTeacherName;
    label->setText(QString::fromStdString(logged.getFullname()));
    label->setAlignment(Qt::AlignCenter);
    on_btnDashboard_clicked();
}

StudentForm::~StudentForm()
{
    delete ui;
}

void StudentForm::on_btnDashboard_clicked()
{
    QGroupBox *grBox = ui->groupBoxDashboard;
    grBox->show();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxHistory;
    grBox->hide();

    QLabel *label = ui->lbWarning;
    label->hide();

    QTableWidget *table = ui->tbTeacherDashboard;
    table->setColumnCount(8);
    table->setRowCount(0);
    table->clearContents();
    table->setHorizontalHeaderLabels({"Test ID", "Teacher ID", "Title", "Total Question", "Duration", "Start At", "End At", "Enter"});
    table->setColumnWidth(0, 60);
    table->setColumnWidth(1, 60);
    table->setColumnWidth(2, 120);
    table->setColumnWidth(3, 90);
    table->setColumnWidth(4, 60);
    table->setColumnWidth(5, 140);
    table->setColumnWidth(6, 140);
    table->setColumnWidth(7, 60);
    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
    header->setStretchLastSection(true);

    int runningTestCount = 0;
    Test *runningTest = testManager.getRunningTest(logged.getId(), runningTestCount);
    if (runningTestCount == 0)
    {
        label->show();
        return;
    }
    for (int i = 0; i < runningTestCount; i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(runningTest[i].getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(runningTest[i].getTeacherId())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(runningTest[i].getTitle())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(runningTest[i].getTotalQuestion())));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(runningTest[i].getDuration())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(runningTest[i].getStartsAt())));
        table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(runningTest[i].getEndsAt())));
        for (int j = 0; j < 7; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }

        QPushButton *btn = new QPushButton("Enter");
        btn->setStyleSheet("background-color: #2D2D3B; color: #FFFFFF; border: none; padding: 10px; text-align: center;");
        table->setCellWidget(i, 7, btn);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet("padding: 5px 5px; font-size: 8px; border-radius: 5px; border: 1px solid #ddd; background-color: #007BFF; color: white; margin: 2px; min-width: 30px; max-width: 30px; min-height: 15px; max-height: 15px; text-align: center; cursor: pointer; align-items: center; outline: none;");
        connect(btn, &QPushButton::clicked, this, [this, runningTest, i]()
                {
                QInputDialog* dialog = new QInputDialog(this);
                dialog->setStyleSheet("color: white; background-color: black;");
                dialog->setWindowTitle("Enter Test");
                dialog->setLabelText("Enter password to start the test:");
                dialog->setOkButtonText("Enter");
                dialog->exec();
                QString inputDialogText = dialog->textValue();
                if (inputDialogText == QString::fromStdString(runningTest[i].getPassword())) {
                    QMessageBox qmb;
                    qmb.setWindowTitle("Enter Test");
                    qmb.setText("Do you want to start the test?");
                    qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    qmb.setStyleSheet("QMessageBox { background-color: black; color: white; }"
                                    "QPushButton { background-color: gray; color: black; }");
                    QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(qmb.exec());
                    if (reply == QMessageBox::Yes)
                    {
                        QGroupBox *grBox = ui->groupBoxIntoTest;
                        grBox->show();
                        grBox = ui->groupBoxDashboard;
                        grBox->hide();
                        grBox = ui->leftSidebar;
                        grBox->hide();
                        setUpTestForm(runningTest[i]);
                    }
                } else {
                    QMessageBox warningBox(this);
                    warningBox.setIcon(QMessageBox::Warning); // Đặt biểu tượng cảnh báo
                    warningBox.setWindowTitle("Fail");
                    warningBox.setText("Incorrect password!");
                    warningBox.setStyleSheet(
                        "QMessageBox { background-color: #fff3f3; color: black; }"  // Nền màu đỏ nhạt, chữ màu đen
                        "QLabel { color: black; font-size: 14px; }"                 // Tùy chỉnh font chữ
                        "QPushButton { background-color: #ffcccc; color: black; border: 1px solid gray; padding: 5px; }"
                        "QPushButton:hover { background-color: #ff9999; }"          // Hiệu ứng hover
                    );

                    warningBox.exec(); // Hiển thị hộp thoại

                } });
    }
}
void StudentForm::setUpTestForm(Test test)
{
    StudentAttempt *currentAttempt = studentAttemptManager.createAttempt(test.getId(), logged.getId(), test.getTotalQuestion(), test.getDuration(), test.getTeacherId());
    if (currentAttempt->getId() == "")
    {
        QMessageBox warningBox(this);
        warningBox.setIcon(QMessageBox::Warning); // Đặt biểu tượng cảnh báo
        warningBox.setWindowTitle("Error");
        warningBox.setText("Cannot create attempt!");
        warningBox.setStyleSheet(
            "QMessageBox { background-color: #fff4e6; color: black; }" // Nền màu cam nhạt, chữ màu đen
            "QLabel { color: black; font-size: 14px; }"                // Tùy chỉnh font chữ
            "QPushButton { background-color: #ffd9b3; color: black; border: 1px solid gray; padding: 5px; }"
            "QPushButton:hover { background-color: #ffc299; }" // Hiệu ứng hover
        );

        warningBox.exec(); // Hiển thị hộp thoại

        return;
    }
    CountdownTimer(test.getDuration() * 60);

    QTableWidget *table = ui->tbQuestion;
    table->clearContents();
    table->setRowCount(0);
    table->setColumnCount(5);
    table->setColumnWidth(0, 45);
    table->setColumnWidth(1, 45);
    table->setColumnWidth(2, 45);
    table->setColumnWidth(3, 45);
    table->setColumnWidth(4, 45);

    table->setHorizontalHeaderLabels({"", "", "", "", ""});
    int rowCount = (test.getTotalQuestion() % 5) ? test.getTotalQuestion() / 5 + 1 : test.getTotalQuestion() / 5;
    for (int i = 0; i < rowCount; i++)
    {
        table->insertRow(i);
        int columnCount = (i == rowCount - 1) && (test.getTotalQuestion() % 5) ? test.getTotalQuestion() % 5 : 5;
        for (int j = 0; j < columnCount; j++)
        {
            QPushButton *button = new QPushButton(QString::number(i * 5 + j + 1));
            button->setStyleSheet("background-color: red; color: white; padding: 5px;");
            table->setCellWidget(i, j, button);
            connect(button, &QPushButton::clicked, this, [this, i, j, test, currentAttempt, button]()
                    {
                        int questionIndex = i * 5 + j;
                        Question *question = questionBankManager.getQuestionById(currentAttempt->getQuestionId(questionIndex));

                        QLabel *label = ui->lbQuestion;
                        label->setText(QString::fromStdString(question->getQuestionText()));
                        label->setWordWrap(true);
                        label = ui->lbNumber;
                        label->setText("Question " + QString::number(questionIndex + 1));
                        label->setAlignment(Qt::AlignCenter);
                        for (int i = 0; i < ui->layoutOption->count(); ++i)
                        {
                            QWidget *widget = ui->layoutOption->itemAt(i)->widget();
                            if (widget) {
                                widget->deleteLater();
                            }
                        }
                        int studentAnswer = currentAttempt->getSudentAnswer(questionIndex);
                        // Thêm radio button mới
                        for (int k = 0; k < question->getNumberOfOptions(); k++)
                        {
                            QRadioButton *radio = new QRadioButton();
                            radio->setText(QString::fromStdString(question->getOption(k)));
                            if (k == studentAnswer - 1)
                            {
                                radio->setChecked(true);
                            }
                            connect(radio, &QRadioButton::clicked, this, [this, questionIndex, k, currentAttempt, button](){
                                studentAttemptManager.setStudentAnswer(currentAttempt, questionIndex, k + 1);
                                button->setStyleSheet("background-color: #4CAF50; color: white; padding: 5px;");
                            });
                            radio->setStyleSheet(
                                "QRadioButton {"
                                "    background-color: #f0f0f0;" 
                                "    border-radius: 10px;"       
                                "    padding: 5px;"    
                                "    color: black;"        
                                "}"
                                "QRadioButton::indicator {"
                                "    width: 15px;"          
                                "    height: 15px;"
                                "    border-radius: 7px;"       
                                "    border: 2px solid #0078d7;"
                                "    background-color: white;" 
                                "}"
                                "QRadioButton::indicator:checked {"
                                "    background-color: #0078d7;" 
                                "    border: 2px solid #005bb5;" 
                                "}"
                            );
                            ui->layoutOption->addWidget(radio);
                        } });
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setFocusPolicy(Qt::NoFocus);
}
void StudentForm::on_btnLogoutTeacherDashboard_clicked()
{
    this->hide();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    logged = currentUser("", "", "", "", "");
}

void StudentForm::CountdownTimer(int startTime)
{
    timeRemaining = startTime;

    ui->lcdNumber->setDigitCount(5);

    std::thread([this]()
                {
        while (timeRemaining > 0) {
            int minutes = timeRemaining / 60;
            int seconds = timeRemaining % 60;
            QMetaObject::invokeMethod(this, [this, minutes, seconds]() {
                ui->lcdNumber->display(QString("%1:%2")
                                           .arg(minutes, 2, 10, QChar('0'))
                                           .arg(seconds, 2, 10, QChar('0')));
            });
            std::this_thread::sleep_for(std::chrono::seconds(1));
            timeRemaining--;
        }
        QMetaObject::invokeMethod(this, [this]() {
            ui->lcdNumber->display("00:00");
            QMessageBox *qmb = new QMessageBox();
            qmb->setText("Time's up!");
            qmb->setStyleSheet("background-color: black; color: white;");
            qmb->exec();
            studentAttemptManager.setFinishedAtForLastAttempt();
            setHistoryTable();
        }); })
        .detach(); // Tách thread để không block main thread
}
void StudentForm::setHistoryTable()
{
    TestManager testManager;

    QGroupBox *grBox = ui->groupBoxHistory;
    grBox->show();
    grBox = ui->leftSidebar;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxIntoTest;
    grBox->hide();
    QTableWidget *table = ui->tbHistory;
    table->setColumnCount(6);
    table->setRowCount(0);
    table->clearContents();
    table->setHorizontalHeaderLabels({"Test ID", "Title", "Total Question", "Correct Answers", "Start At", "End At"});
    table->setColumnWidth(0, 60);
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 100);
    table->setColumnWidth(3, 100);
    table->setColumnWidth(4, 140);
    table->setColumnWidth(5, 140);
    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
    table->setStyleSheet("QTableWidget { background-color: white; color: black; }");
    header->setStretchLastSection(true);

    int historyTestCount = 0;
    StudentAttempt *historyTest = studentAttemptManager.getAttemptsByStudentId(logged.getId(), historyTestCount);

    for (int i = 0; i < historyTestCount; i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(historyTest[i].getTestId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(testManager.getTestById(historyTest[i].getTestId())->getTitle())));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(historyTest[i].getTotalQuestions())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(historyTest[i].getCorrectAnswer())));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(historyTest[i].getStartsAt())));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(historyTest[i].getFinishedAt())));

        for (int j = 0; j < 6; j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
void StudentForm::on_pushButton_clicked()
{
    QMessageBox qmb;
    qmb.setWindowTitle("Submit Test");
    qmb.setText("Are you sure you want to submit the test?");
    qmb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    qmb.setStyleSheet("QMessageBox { background-color: black; color: white; }"
                      "QPushButton { background-color: gray; color: black; }");

    QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(qmb.exec());

    if (reply == QMessageBox::Yes)
    {
        // Stop the countdown timer
        timeRemaining = 0;

        studentAttemptManager.setFinishedAtForLastAttempt();
        setHistoryTable();
    }
}

void StudentForm::on_btnAddNewTest_clicked()
{
    QGroupBox *grBox = ui->groupBoxHistory;
    grBox->show();
    setHistoryTable();
    grBox = ui->leftSidebar;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->hide();
    grBox = ui->groupBoxIntoTest;
    grBox->hide();
}

void StudentForm::on_btnEditProfile_clicked()
{
    QGroupBox *grBox = ui->groupBoxIntoTest;
    grBox->hide();
    grBox = ui->groupBoxEditProfile;
    grBox->show();
    grBox = ui->leftSidebar;
    grBox->show();
    grBox = ui->groupBoxDashboard;
    grBox->hide();
    grBox = ui->groupBoxHistory;
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
void StudentForm::on_btnEditProfileSubmit_clicked()
{
    QString qFullname = ui->txtEditProfileFullname->text();
    QString qPassword = ui->txtEditProfilePassword->text();

    string fullname = qFullname.toStdString();
    string password = qPassword.toStdString();

    if (managerStudent.update(logged.getId(), password, fullname))
    {
        QMessageBox infoBox(this);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.setWindowTitle("Edit Profile");
        infoBox.setText("Edit profile successful!");
        infoBox.setStyleSheet(
            "QMessageBox { background-color: #black; color: white; }" // Nền xanh nhạt, chữ màu đen
            "QLabel { color: black; font-size: 14px; }"               // Tùy chỉnh font chữ
            "QPushButton { background-color: #d9f2ff; color: black; border: 1px solid gray; padding: 5px; }"
            "QPushButton:hover { background-color: #b3e6ff; }" // Hiệu ứng hover
        );

        infoBox.exec(); // Hiển thị hộp thoại

        on_btnDashboard_clicked();
        QLabel *label = ui->lbTeacherName;
        label->setText(QString::fromStdString(fullname));
        label->setAlignment(Qt::AlignCenter);
        logged.setFullname(fullname);
        logged.setPassword(password);
    }
    else
    {
        QMessageBox warningBox(this);
        warningBox.setIcon(QMessageBox::Warning);
        warningBox.setWindowTitle("Edit Profile");
        warningBox.setText("Form is incorrect. Please try again!");
        warningBox.setStyleSheet(
            "QMessageBox { background-color: black; color: white; }" // Nền màu đỏ nhạt
            "QPushButton { background-color: #f5f5f5; color: black; border: 1px solid gray; }"
            "QPushButton:hover { background-color: #d6d6d6; }" // Hiệu ứng hover
        );

        warningBox.exec(); // Hiển thị hộp thoại
    }
}
