#ifndef PROCESSWIDGET_H
#define PROCESSWIDGET_H

#include <QWidget>

namespace Ui {
class ProcessWidget;
}

class ProcessWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessWidget(QWidget *parent = 0);
    ~ProcessWidget();

private slots:
    void on_btn_Cancel_clicked();

private:
    Ui::ProcessWidget *ui;
    QGridLayout *layout;
signals:
    void processCanceled();
};

#endif // PROCESSWIDGET_H
