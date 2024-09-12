#ifndef TEACHERDASHBOARDWIDGET_H
#define TEACHERDASHBOARDWIDGET_H

#include <QWidget>

namespace Ui {
class TeacherDashboardWidget;
}

class TeacherDashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherDashboardWidget(QWidget *parent = nullptr);
    ~TeacherDashboardWidget();

private:
    Ui::TeacherDashboardWidget *ui;
};

#endif // TEACHERDASHBOARDWIDGET_H
