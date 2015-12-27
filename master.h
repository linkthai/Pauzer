#ifndef MASTER
#define MASTER

#include "stable.h"
#include <string>

class Master
{
private:
    QString _name;
    int _songCount;
    QMap<int, QString> _list;
public:
	Master(void);
	~Master(void);
    void AddToList(int ID, QString path);
    void SetList(QMap<int, QString> map);
    void RemoveFromList(int id);
    QString Get(int id);
    void SetName(QString name);
    QString GetName();
	int GetCount();
    void ClearList();
};

#endif // MASTER

