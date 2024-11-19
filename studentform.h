#ifndef STUDENTFORM_H
#define STUDENTFORM_H
#include <QWidget>
#include "./src/currentUser/currentUser.h"

namespace Ui
{
    class StudentForm;
}

class StudentForm : public QWidget
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = nullptr);
    ~StudentForm();

private slots:
    void on_btnDashboard_clicked();
    void on_btnLogoutTeacherDashboard_clicked();

private:
    Ui::StudentForm *ui;
    currentUser logged;
};

#endif // STUDENTFORM_H
