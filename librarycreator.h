#ifndef LIBRARYCREATOR_H
#define LIBRARYCREATOR_H

#include <QThread>
#include "manager.h"
#include "master.h"

class LibraryCreator : public QThread
{
    Q_OBJECT
public:
    LibraryCreator(QObject *parent = 0);
    void setList(QStringList folders);
private:
    QStringList list;
    void run();
};

#endif // LIBRARYCREATOR_H
