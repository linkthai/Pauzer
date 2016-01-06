#ifndef MASTERTABLEVIEW_H
#define MASTERTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>

class MasterTableView : public QTableView
{
    Q_OBJECT
public:
    MasterTableView(QWidget *parent = 0);
};

#endif // MASTERTABLEVIEW_H
