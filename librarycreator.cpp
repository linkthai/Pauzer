#include "librarycreator.h"

LibraryCreator::LibraryCreator(QObject *parent) :
    QThread(parent)
{

}

void LibraryCreator::setList(QStringList folders)
{
    list.clear();
    for (int i = 0; i < folders.size(); i++)
    {
        list.append(folders.at(i));
    }
}

void LibraryCreator::run()
{
    Manager::CreateMaster(list);
}

