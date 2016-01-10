#ifndef MASTERTABLEVIEW_H
#define MASTERTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

#include "mastermodel.h"

class MasterTableView : public QTableView
{
    Q_OBJECT
public:
    MasterTableView(QWidget *parent = 0);
public slots:
    void ShowContextMenu(const QPoint& pos);
    void playPlaylist();
    void queuePlaylist();
};

#endif // MASTERTABLEVIEW_H
