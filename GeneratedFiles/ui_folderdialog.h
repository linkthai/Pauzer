/********************************************************************************
** Form generated from reading UI file 'folderdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLDERDIALOG_H
#define UI_FOLDERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_FolderDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTreeView *treeView;

    void setupUi(QDialog *FolderDialog)
    {
        if (FolderDialog->objectName().isEmpty())
            FolderDialog->setObjectName(QStringLiteral("FolderDialog"));
        FolderDialog->resize(400, 500);
        buttonBox = new QDialogButtonBox(FolderDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 441, 331, 51));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        treeView = new QTreeView(FolderDialog);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(30, 40, 341, 391));

        retranslateUi(FolderDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), FolderDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FolderDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FolderDialog);
    } // setupUi

    void retranslateUi(QDialog *FolderDialog)
    {
        FolderDialog->setWindowTitle(QApplication::translate("FolderDialog", "Select folders", 0));
    } // retranslateUi

};

namespace Ui {
    class FolderDialog: public Ui_FolderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLDERDIALOG_H
