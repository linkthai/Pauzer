#ifndef XML_PARSER_H
#define XML_PARSER_H
#include "stable.h"
#include "xml_bus.h"
#include "xml_data.h"
#include "song.h"
#include "master.h"

class Xml_Parser
{
public:
    Xml_Parser();
    QStringList GetAllAlbums();
    QStringList GetAllArtist();
    QStringList GetAlbumsByArtist(int ID);
    QList<Song> GetSongsByAlbum(int ID);
    int GetAllAlbumsCount();
    int GetAllArtistsCount();
    int GetAllAlbumsCountByArtist(int ID);
    void GetSongsInPlaylist(Master &list);

    void Init();
    void SavePlaylist();
    void SaveChoosenFolder(QString path);
    void AddToDom(QString title, QString artist, QString album, QString path);
    void LoadData(QString path);
private:
    QDomElement root;
    unsigned int albID;
    unsigned int artID;
};

#endif // XML_PARSER_H
