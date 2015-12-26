#ifndef FOLDERDIALOG_H
#define FOLDERDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QDesktopServices>
#include "checkablefilesystemmodel.h"

namespace Ui {
class FolderDialog;
}

class FolderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FolderDialog(QWidget *parent = 0);
    ~FolderDialog();
private slots:
    void accept();
signals:
    void createdFolderList(QStringList list);
private:
    Ui::FolderDialog *ui;

    CheckableFileSystemModel *dirModel;
    QStringList list;
};

#endif // FOLDERDIALOG_H
