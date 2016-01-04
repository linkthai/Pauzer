#include "Master.h"


Master::Master(void)
{
	_songCount = 0;
}

Master::~Master(void)
{
}

void Master::AddToList(int ID, QString path)
{
    _list.insert(ID, path);
	_songCount++;
}

void Master::SetList(QMap<int, QString> map)
{
    _list = map;
}

void Master::SetAlbumList(QMap<QString, QMap<int, QString> > map)
{
    _albumList = map;
}

void Master::RemoveFromList(int id)
{
    _list.remove(id);
    _songCount--;
}

QString Master::Get(int id)
{
    return _list.value(id);
}
void Master::SetName(QString name)
{
    _name = name;
}

QString Master::GetName()
{
    return _name;
}

int Master::GetCount()
{
    return _list.size();
}

int Master::GetAlbumsCount()
{
    return _albumList.size();
}

void Master::ClearList()
{
    _songCount = 0;
    _list.clear();
}

void Master::ClearAlbumList()
{
    _albumList.clear();
}
