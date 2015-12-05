#ifndef CHECKABLEFILESYSTEMMODEL_H
#define CHECKABLEFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QStandardItem>

class CheckableFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    CheckableFileSystemModel();
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role );
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
    virtual QVariant data( const QModelIndex &index, int role )const;
    QStringList getFolders();
signals:
    void itemChecked(const QModelIndex&);
protected slots:
    void onItemChecked(const QModelIndex& index);
private:
    mutable QMap <qint64, Qt::CheckState> checkedItems;
    void getString(QStringList &list, QModelIndex &index);
};

#endif // CHECKABLEFILESYSTEMMODEL_H
