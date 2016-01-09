#ifndef XML_PARSER_H
#define XML_PARSER_H
#include "stable.h"
#include "xml_bus.h"
#include "xml_data.h"
#include "master.h"

class Xml_Parser
{
    template <typename T1, typename T2>
    struct less_second {
        typedef QPair<T1, T2> type;
        bool operator ()(type const& a, type const& b) const {
            return a.second < b.second;
        }
    };

    struct AlbumData
    {
        QStringList album;
        QStringList artist;
        QStringList year;
        QStringList id;
    };

public:
    Xml_Parser();
    QMap<QString, QStringList> GetAllAlbums();
    QList<QPair<int, QString>> GetAllArtist();
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

    void sortMap(QMap<QString, QStringList> &map);
    void quicksort(int leftIdx, int rightIdx, AlbumData &data);
    void swap(int leftIdx, int rightIdx, AlbumData &data);
    int divide(int leftIdx, int rightIdx, AlbumData &data);
private:
    QDomElement root;
    unsigned int albID;
    unsigned int artID;
    unsigned int songID;
};

#endif // XML_PARSER_H
