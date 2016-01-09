#ifndef XML_PARSER_H
#define XML_PARSER_H
#include "stable.h"
#include "xml_bus.h"
#include "xml_data.h"
#include "master.h"

class Xml_Parser
{
public:
    Xml_Parser();
    QMap<QString, QMap<int, QString>> GetAllAlbums();
    QStringList GetAllArtist();
    QMap<int, QString> GetAllSong();

    QList<int> GetAlbumsByArtist(int ID);
    QList<int> GetSongsByAlbum(int ID);

    QString GetArtistNameByID(int ID);
    QString GetAlbumNameByID(int ID);
    QString GetSongPathByID(int ID);

    int GetAllAlbumsCount();
    int GetAllArtistsCount();
    int GetAllSongsCount();
    int GetAllSongsCountByAlbum(int ID);
    int GetAllAlbumsCountByArtist(int ID);

    void GetSongsInPlaylist(Master &list);
    void GetAlbumsInPlaylist(Master &list);

    void Init();
    void SavePlaylist();
    void SaveChoosenFolder(QString path);
    void AddToDom(QString title, QString artist, QString album, QString path, uint year);
    void LoadData(QString path);

    void sortMap(QMap<int, QString> &map);
private:
    QDomElement root;
    unsigned int albID;
    unsigned int artID;
    unsigned int songID;
};

#endif // XML_PARSER_H
