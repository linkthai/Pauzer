#include "folderdialog.h"
#include "ui_folderdialog.h"

FolderDialog::FolderDialog(QWidget *parent) :
    QDialog(parent),
     ui(new Ui::FolderDialog)
{
    ui->setupUi(this);

    // Creates our new model and populate
    QString mPath = "C:/";
    dirModel = new CheckableFileSystemModel();

    // Set filter
    dirModel->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);

    // QFileSystemModel requires root path
    dirModel->setRootPath(mPath);

    // Attach the model to the view
    ui->treeView->setModel(dirModel);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

FolderDialog::~FolderDialog()
{
    delete ui;
}

void FolderDialog::accept()
{
    list = dirModel->getFolders();

    if (list.length() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select at least one folder!");
        msgBox.exec();
    }
    else
    {
        emit createdFolderList(list);
        this->close();
    }
}
