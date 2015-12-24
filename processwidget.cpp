#include "processwidget.h"
#include "ui_processwidget.h"

ProcessWidget::ProcessWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setFixedSize(220, 150);
    this->setStyleSheet("QDialog#ProcessWidget {"
                        "background-color: #333333;"
                        "border: 1px solid #666666;"
                        "}");

    layout = new QGridLayout(this);

    layout->addWidget(ui->lbl_Process, 0, 0, 1, -1, Qt::AlignCenter);
    layout->addWidget(ui->btn_Cancel, 1, 0, 1, -1, Qt::AlignCenter);

    ui->lbl_Process->setMargin(30);
    ui->btn_Cancel->setFlat(true);
    ui->btn_Cancel->setFixedSize(80, 30);

    ui->lbl_Process->setStyleSheet("color: #e6e6e6");
    ui->btn_Cancel->setStyleSheet("QPushButton {"
                                  "border: 0px;"
                                  "background-color: #4d4d4d;"
                                  "color: #999999;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #737373;"
                                  "color: #bfbfbf;"
                                  "}");
}

ProcessWidget::~ProcessWidget()
{
    delete ui;
}

void ProcessWidget::on_btn_Cancel_clicked()
{
    emit processCanceled();
    this->close();
}
