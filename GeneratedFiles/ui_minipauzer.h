/********************************************************************************
** Form generated from reading UI file 'minipauzer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIPAUZER_H
#define UI_MINIPAUZER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>
#include "sliderbar.h"

QT_BEGIN_NAMESPACE

class Ui_MiniPauzer
{
public:
    QPushButton *btn_Play;
    QPushButton *btn_ChooseFolders;
    QCheckBox *btn_play;
    QSplitter *splitter;
    QLabel *lbl_CurTime;
    SliderBar *sliderBar;
    QLabel *lbl_songLen;

    void setupUi(QWidget *MiniPauzer)
    {
        if (MiniPauzer->objectName().isEmpty())
            MiniPauzer->setObjectName(QStringLiteral("MiniPauzer"));
        MiniPauzer->resize(669, 258);
        btn_Play = new QPushButton(MiniPauzer);
        btn_Play->setObjectName(QStringLiteral("btn_Play"));
        btn_Play->setGeometry(QRect(360, 200, 91, 31));
        btn_ChooseFolders = new QPushButton(MiniPauzer);
        btn_ChooseFolders->setObjectName(QStringLiteral("btn_ChooseFolders"));
        btn_ChooseFolders->setGeometry(QRect(10, 220, 151, 25));
        btn_play = new QCheckBox(MiniPauzer);
        btn_play->setObjectName(QStringLiteral("btn_play"));
        btn_play->setGeometry(QRect(210, 150, 31, 31));
        splitter = new QSplitter(MiniPauzer);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(200, 80, 441, 22));
        splitter->setOrientation(Qt::Horizontal);
        lbl_CurTime = new QLabel(splitter);
        lbl_CurTime->setObjectName(QStringLiteral("lbl_CurTime"));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        font.setPointSize(9);
        lbl_CurTime->setFont(font);
        lbl_CurTime->setAlignment(Qt::AlignCenter);
        splitter->addWidget(lbl_CurTime);
        sliderBar = new SliderBar(splitter);
        sliderBar->setObjectName(QStringLiteral("sliderBar"));
        sliderBar->setOrientation(Qt::Horizontal);
        splitter->addWidget(sliderBar);
        lbl_songLen = new QLabel(splitter);
        lbl_songLen->setObjectName(QStringLiteral("lbl_songLen"));
        lbl_songLen->setFont(font);
        lbl_songLen->setAlignment(Qt::AlignCenter);
        splitter->addWidget(lbl_songLen);

        retranslateUi(MiniPauzer);

        QMetaObject::connectSlotsByName(MiniPauzer);
    } // setupUi

    void retranslateUi(QWidget *MiniPauzer)
    {
        MiniPauzer->setWindowTitle(QApplication::translate("MiniPauzer", "MiniPauzer", 0));
        btn_Play->setText(QApplication::translate("MiniPauzer", "Play", 0));
        btn_ChooseFolders->setText(QApplication::translate("MiniPauzer", "Choose Music Folders", 0));
        btn_play->setText(QString());
        lbl_CurTime->setText(QApplication::translate("MiniPauzer", "0:00", 0));
        lbl_songLen->setText(QApplication::translate("MiniPauzer", "0:00", 0));
    } // retranslateUi

};

namespace Ui {
    class MiniPauzer: public Ui_MiniPauzer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIPAUZER_H
