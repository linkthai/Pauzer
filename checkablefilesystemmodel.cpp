#include "checkablefilesystemmodel.h"

CheckableFileSystemModel::CheckableFileSystemModel()
{
    connect(this, SIGNAL(itemChecked(const QModelIndex&)), SLOT(onItemChecked(const QModelIndex&)));
}

bool CheckableFileSystemModel::setData( const QModelIndex& index, const QVariant& value, int role ) {

    if (role == Qt::CheckStateRole) {
        /* set indicator in your backing store according to whether value == Qt::Checked */
        checkedItems[index.internalId()] =  static_cast<Qt::CheckState>( value.toInt() );

        emit itemChecked(index);
        emit dataChanged(index.child(0, 0), index.child(index.row() - 1, 0));
        return true;
    }
    return QFileSystemModel::setData(index, value, role);

}

Qt::ItemFlags CheckableFileSystemModel::flags( const QModelIndex & index ) const {

    return QFileSystemModel::flags( index ) | Qt::ItemIsUserCheckable;

}

QVariant CheckableFileSystemModel::data( const QModelIndex& index, int role ) const {

    if( role == Qt::CheckStateRole && index.column()==0 ) {
        /* determine whether the box should be checked or unchecked */

        if (index.parent().isValid())
        {
            Qt::CheckState state = static_cast<Qt::CheckState>( this->data(index.parent(), role).toInt() );
            if (state == Qt::Unchecked || state == Qt::Checked)
            {
                return state;
            }
            else
            {
                return checkedItems.value(index.internalId(), Qt::Unchecked);
            }
        }
        else
        {
            return checkedItems.value(index.internalId(), Qt::Unchecked);
        }
    }

    return QFileSystemModel::data( index, role );
}

QStringList CheckableFileSystemModel::getFolders()
{
    QStringList list;
    for (int i = 0; i < this->rowCount(); i++)
        getString(list, this->index(i, 0));
    return list;
}

void CheckableFileSystemModel::getString(QStringList &list, QModelIndex &index)
{
    Qt::CheckState state = static_cast<Qt::CheckState>( this->data(index, Qt::CheckStateRole).toInt() );

    if (state == Qt::Checked)
        list.append(this->filePath(index));

    if (state == Qt::PartiallyChecked)
    {
        for (int i = 0; i < rowCount(index); i++)
            getString(list, index.child(i, 0));
    }

    return;
}

void CheckableFileSystemModel::onItemChecked(const QModelIndex &index)
{
    fetchMore(index);

    QModelIndex parent = index.parent();
    Qt::CheckState parentState = checkedItems.value(parent.internalId(), Qt::Unchecked);
    if (parent.isValid())
    {
        bool allUnchecked = true;
        bool allChecked = true;
        Qt::CheckState state;
        for (int i = 0; i < rowCount(parent); i++)
        {
            //state = static_cast<Qt::CheckState>( this->data(index.sibling(i, 0), Qt::CheckStateRole).toInt() );
            state = checkedItems.value(index.sibling(i, 0).internalId(), Qt::PartiallyChecked);

            if (allUnchecked == true && state != Qt::Unchecked)
                allUnchecked = false;
            if (allChecked == true && state != Qt::Checked)
                allChecked = false;
            if (allUnchecked == false && allChecked == false)
                break;
        }

        if (allUnchecked == true)
        {
            setData(parent, Qt::Unchecked, Qt::CheckStateRole);
        }
        else
        {
            if (allChecked == true)
            {
                setData(parent, Qt::Checked, Qt::CheckStateRole);
            }
            else
            {
                if (parentState != Qt::PartiallyChecked)
                {
                    for (int i = 0; i < rowCount(parent); i++)
                    {
                        if (i == index.row())
                            continue;

                        if (parentState == Qt::Checked)
                            checkedItems[parent.child(i, 0).internalId()] = Qt::Checked;
                        else
                            checkedItems[parent.child(i, 0).internalId()] = Qt::Unchecked;


                    }
                }
                setData(parent, Qt::PartiallyChecked, Qt::CheckStateRole);
            }
        }
    }

}
