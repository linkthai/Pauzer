#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QObject *parent = 0);
private:

signals:

public slots:
};

#endif // ARTIST_H
