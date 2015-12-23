#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include "stable.h"
#include "song.h"

class Album : public QObject
{
    Q_OBJECT
public:
    explicit Album(QObject *parent = 0);
//    Album(void);
//    ~Album(void);
    void AddToList(Song song);
    void RemoveFromList(std::wstring id);
    void RemoveFromList(int pos);
    Song Get(int pos);
    Song Get(std::wstring id);
    void SetName(std::wstring name);
    std::wstring GetName();
    int GetCount();
private:
    std::wstring _id;
    std::wstring _name;
    int _songCount;
    //QMap<Song> _list;
signals:

public slots:
};

#endif // ALBUM_H
