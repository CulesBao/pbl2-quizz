#include "studentform.h"
#include "ui_studentform.h"
#include "mainwindow.h"
#include "./src/currentUser/currentUser.h"
#include <QDebug>
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
    on_btnDashboard_clicked();

    qDebug() << "StudentForm" << logged.getUsername();
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
    table->setColumnWidth(0, 80);
    table->setColumnWidth(1, 80);
    table->setColumnWidth(2, 120);
    table->setColumnWidth(3, 80);
    table->setColumnWidth(4, 60);
    table->setColumnWidth(5, 160);
    table->setColumnWidth(6, 160);
    table->setColumnWidth(7, 60);
    QHeaderView *header = table->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
}

void StudentForm::on_btnLogoutTeacherDashboard_clicked()
{
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
    logged = currentUser("", "", "", "", "");
}
