#ifndef MASTER_H
#define MASTER_H

#include "stable.h"
#include <string>

class Master
{
private:
    QString _name;
    int _songCount;
    QMap<int, QString> _list;
    QMap<QString, QStringList> _albumList;
public:
	Master(void);
	~Master(void);
    void AddToList(int ID, QString path);
    void SetList(QMap<int, QString> map);
    void SetAlbumList(QMap<QString, QStringList> map);
    void RemoveFromList(int id);
    QString Get(int id);
    void SetName(QString name);
    QString GetName();
	int GetCount();
    int GetAlbumsCount();
    void ClearList();
    void ClearAlbumList();
};

#endif // MASTER

