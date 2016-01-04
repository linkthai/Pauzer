#include "xml_parser.h"

Xml_Parser::Xml_Parser()
{

}
void Xml_Parser::Init()
{
    Xml_Bus BUS;
    albID = 0;
    artID = 0;
    songID = 0;
    root = BUS.creatRoot("DATA");
}
QMap<QString, QMap<int, QString>> Xml_Parser::GetAllAlbums()
{
    QMap<QString, QMap<int, QString>> mainList;

    QDomNodeList albums = root.elementsByTagName("ALBUM");
    QString header;

    for (int t = 0; t < 3; t++)
    {
        QMap<int, QString> subList;

        switch (t)
        {
        case 0:
            header = "Album_name";
            break;
        case 1:
            header = "Artist_name";
            break;
        case 2:
            header = "Year";
            break;
        }

        for (int i = 0; i < albums.length(); i++)
        {
            subList.insert(albums.at(i).toElement().attribute("ID").toInt(), albums.at(i).toElement().attribute(header));
        }

        mainList.insert(header, subList);
    }
    return mainList;
}
QStringList Xml_Parser::GetAllArtist()
{
    QStringList allArtist;
    QDomNodeList artists = root.elementsByTagName("ARTIST");
    for (int i = 0; i < artists.length(); i++)
    {
        allArtist.push_back(artists.at(i).toElement().attribute("Artist_name"));
    }
    return allArtist;
}
QList<int> Xml_Parser::GetAlbumsByArtist(int ID)
{
    QList<int> allAlbums;
    QDomNodeList artists = root.elementsByTagName("ARTIST");
    for (int i = 0; i < artists.length(); i++)
    {
        if (artists.at(i).toElement().attribute("ID").toInt() == ID)
        {
            for (int j = 0; j < artists.at(i).toElement().childNodes().length(); j++)
                allAlbums.push_back(artists.at(i).toElement().childNodes().at(j).toElement().attribute("ID").toInt());
        }
    }
    return allAlbums;
}
QList<int> Xml_Parser::GetSongsByAlbum(int ID)
{
    QDomElement songList;
    QList<int> list;
    Xml_Bus BUS;
    songList = BUS.creatRoot("SONG_LIST");
    QDomNodeList albums = root.elementsByTagName("ALBUM");

    for (int i = 0; i < albums.length(); i++)
    {
        if (albums.at(i).toElement().attribute("ID").toInt() == ID)
            BUS.GetAllSong(albums.at(i).toElement(), songList);
    }
    for (int i = 0; i < songList.childNodes().length(); i++)
    {
        list.push_back(songList.childNodes().at(i).toElement().attribute("ID").toInt());
    }

    return list;
}
int Xml_Parser::GetAllAlbumsCount()
{
    return root.elementsByTagName("ALBUM").length();
}
int Xml_Parser::GetAllArtistsCount()
{
    return root.elementsByTagName("ARTIST").length();
}
int Xml_Parser::GetAllAlbumsCountByArtist(int ID)
{
    QDomNodeList artists = root.elementsByTagName("ARTIST");
    for (int i = 0; i < artists.length(); i++)
    {
        if (artists.at(i).toElement().attribute("ID").toInt() == ID)
        {
            return artists.at(i).toElement().childNodes().length();
        }
    }
    return 0;
}
void Xml_Parser::GetSongsInPlaylist(Master &list)
{
    if (list.GetCount() != 0)
        list.ClearList();

    list.SetList(GetAllSong());
}

void Xml_Parser::GetAlbumsInPlaylist(Master &list)
{
    if (list.GetAlbumsCount() != 0)
        list.ClearAlbumList();

    list.SetAlbumList(GetAllAlbums());
}

void SavePlaylist();
void Xml_Parser::AddToDom(QString title, QString artist, QString album, QString path, uint year)
{
    Xml_Bus BUS;
    Xml_Data DATA;
    for (int i = 0; i < root.childNodes().length(); i++)
    {
        QDomElement node = root.childNodes().at(i).toElement();
        if (node.attribute("Artist_name") == artist)
        {
            for (int j = 0; j < node.childNodes().length(); j++)
            {
                QDomElement child = node.childNodes().at(j).toElement();
                if (child.attribute("Album_name") == album)
                {
                    QDomElement temp = BUS.creatNode("SONG", child);
                    temp.setAttribute("ID", songID);
                    temp.setAttribute("Title", title);
                    temp.setAttribute("Path", path);

                    songID++;

                    return;
                }
            }

            QDomElement temp = BUS.creatNode("ALBUM", node);
            temp.setAttribute("ID", albID);
            temp.setAttribute("Album_name", album);
            temp.setAttribute("Artist_name", artist);
            temp.setAttribute("Year", year);
            QDomElement songNode = BUS.creatNode("SONG", temp);
            songNode.setAttribute("ID", songID);
            songNode.setAttribute("Title", title);
            songNode.setAttribute("Path", path);

            albID++;
            songID++;

            return;
        }
    }

    QDomElement temp = BUS.creatNode("ARTIST", root);
    temp.setAttribute("ID", artID);
    temp.setAttribute("Artist_name", artist);
    QDomElement albumNode = BUS.creatNode("ALBUM", temp);
    albumNode.setAttribute("ID", albID);
    albumNode.setAttribute("Album_name", album);
    albumNode.setAttribute("Artist_name", artist);
    albumNode.setAttribute("Year", year);
    QDomElement songNode = BUS.creatNode("SONG", albumNode);
    songNode.setAttribute("ID", songID);
    songNode.setAttribute("Title", title);
    songNode.setAttribute("Path", path);

    albID++;
    artID++;
    songID++;

    return;
}
QMap<int, QString> Xml_Parser::GetAllSong()
{
    QMap<int, QString> list;
    QDomNodeList songs = root.elementsByTagName("SONG");
    for (int i = 0; i < songs.length(); i++)
    {
        list.insert(songs.at(i).toElement().attribute("ID").toInt(), songs.at(i).toElement().attribute("Path"));
    }
    return list;
}
int Xml_Parser::GetAllSongsCount()
{
    QDomNodeList songs = root.elementsByTagName("SONG");
    return songs.count();
}
int Xml_Parser::GetAllSongsCountByAlbum(int ID)
{
    QDomNodeList albums = root.elementsByTagName("ALBUM");
    for (int i = 0; i < albums.length(); i++)
    {
        if (albums.at(i).toElement().attribute("ID").toInt() == ID)
            return albums.at(i).childNodes().length();
    }
    return 0;
}
QString Xml_Parser::GetArtistNameByID(int ID)
{
    QDomNodeList artists = root.elementsByTagName("ARTIST");
    for (int i = 0; i < artists.length(); i++)
    {
        if (artists.at(i).toElement().attribute("ID").toInt() == ID)
        {
            return artists.at(i).toElement().attribute("Artist_name");
        }
    }
    return "";
}
QString Xml_Parser::GetAlbumNameByID(int ID)
{
    QDomNodeList albums = root.elementsByTagName("ALBUM");
    for (int i = 0; i < albums.length(); i++)
    {
        if (albums.at(i).toElement().attribute("ID").toInt() == ID)
        {
            return albums.at(i).toElement().attribute("Album_name");
        }
    }
    return "";
}
QString Xml_Parser::GetSongPathByID(int ID)
{
    QDomNodeList songs = root.elementsByTagName("SONG");
    for (int i = 0; i < songs.length(); i++)
    {
        if (songs.at(i).toElement().attribute("ID").toInt() == ID)
        {
            return songs.at(i).toElement().attribute("Path");
        }
    }
    return "";
}
void Xml_Parser::SaveChoosenFolder(QString path)
{
    Xml_Data DATA;
    DATA.writeXml(root, path);
}
void Xml_Parser::LoadData(QString path)
{
    Xml_Data DATA;
    root = DATA.readXml(path);
}

