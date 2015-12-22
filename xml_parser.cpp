#include "xml_parser.h"

Xml_Parser::Xml_Parser()
{
    Xml_Bus BUS;
    root = BUS.creatRoot("DATA");
}
QStringList Xml_Parser::GetAllAlbums()
{
    QStringList allAlbum;
    QDomNodeList albums = root.elementsByTagName("ALBUM");
    for (int i = 0; i < albums.length(); i++)
    {
        allAlbum.push_back(albums.at(i).toElement().attribute("Album_name"));
    }
    return allAlbum;
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
QStringList Xml_Parser::GetAlbumsByArtist(QString artistName)
{
    QStringList allAlbums;
    QDomNodeList artists = root.elementsByTagName("ARTIST");
    for (int i = 0; i < artists.length(); i++)
    {
        if (artists.at(i).toElement().attribute("Artist_name") == artistName)
        {
            for (int j = 0; j < artists.at(i).toElement().childNodes().length(); j++)
                allAlbums.push_back(artists.at(i).toElement().childNodes().at(j).toElement().attribute("Album_name"));
        }
    }
    return allAlbums;
}
QList<Song> Xml_Parser::GetSongsByAlbum(QString albumName)
{
    QDomElement songList;
    QList<Song> list;
    Xml_Bus BUS;
    QDomNodeList albums = root.elementsByTagName("ALBUM");
    for (int i = 0; i < albums.length(); i++)
    {
        if (albums.at(i).toElement().attribute("Album_name") == albumName)
            BUS.GetAllSong(albums.at(i).toElement(), songList);
    }
    for (int i = 0; i < songList.childNodes().length(); i++)
    {
        Song temp;
        temp.SetPath(songList.childNodes().at(i).toElement().attribute("Path").toStdWString());
        list.push_back(temp);
    }
    return list;
}
QList<Song> Xml_Parser::GetSongsByPlaylist(QString playlistName)
{
    return QList<Song>();
}

void SavePlaylist();
void Xml_Parser::AddToDom(QString title, QString artist, QString album, QString path)
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
                QDomElement child = node.childNodes().at(i).toElement();
                if (child.attribute("Album_name") == album)
                {
                    QDomElement temp = BUS.creatNode("SONG", child);
                    temp.setAttribute("Title", title);
                    temp.setAttribute("Path", path);
                    return;
                }
            }

            QDomElement temp = BUS.creatNode("ALBUM", node);
            temp.setAttribute("Album_name", album);
            QDomElement songNode = BUS.creatNode("SONG", temp);
            songNode.setAttribute("Title", title);
            songNode.setAttribute("Path", path);
            return;
        }
    }

    QDomElement temp = BUS.creatNode("ARTIST", root);
    temp.setAttribute("Artist_name", artist);
    QDomElement albumNode = BUS.creatNode("ALBUM", temp);
    albumNode.setAttribute("Album_name", album);
    QDomElement songNode = BUS.creatNode("SONG", albumNode);
    songNode.setAttribute("Title", title);
    songNode.setAttribute("Path", path);
    return;
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

