#include "librarycreator.h"

LibraryCreator::LibraryCreator(QObject *parent) :
    QThread(parent)
{
    index = 0;
}

void LibraryCreator::setList(QStringList folders)
{
    list.clear();
    for (int i = 0; i < folders.size(); i++)
    {
        list.append(folders.at(i));
    }
}

void LibraryCreator::setIndex(int i)
{
    index = i;
}

void LibraryCreator::run()
{
    switch(index)
    {
    case 0:
        Manager::CreateMaster(list);
        break;
    case 1:
    {
        QMap<QString, QStringList> albumList;
        albumList.clear();
        albumList = Manager::parser.GetAllAlbums();
        emit createdAlbumList(albumList);
    }
        break;
    case 2:
    {
        QList<QPair<int, QString>> artistList;
        artistList = Manager::parser.GetAllArtist();
        emit createdArtistList(artistList);
    }
        break;
    case 3:
    {
        QMap<QString, QMap<int, QString>> songList;

        for (int i = 0; i < 6; i++)
        {
            QString key;
            switch(i)
            {
            case 0:
                key = "Title";
                break;
            case 1:
                key = "Artist";
                break;
            case 2:
                key = "Album";
                break;
            case 3:
                key = "Genre";
                break;
            case 4:
                key = "Year";
                break;
            case 5:
                key = "Duration";
                break;
            }

            QMap<int, QString> subList;

            songList.insert(key, subList);
        }

        songList["Title"].clear();
        songList["Artist"].clear();
        songList["Album"].clear();
        songList["Genre"].clear();
        songList["Year"].clear();
        songList["Duration"].clear();

        QString title, artist, album, genre, year_text, duration_text;
        int hour, minute, sec, length, year;
        QTime duration_time;

        for (int i = 0; i < Manager::master.GetCount(); i++)
        {
            QString filename = Manager::master.Get(i);
            TagLib::MPEG::File f( reinterpret_cast<const wchar_t*>(filename.constData()) );

            if (!f.isValid())
                continue;

            TagLib::ID3v2::Tag *tag = f.ID3v2Tag();

            title = TStringToQString(tag->title());
            artist = TStringToQString(tag->artist());
            album = TStringToQString(tag->album());

            Manager::CheckSongInfo(title, artist, album, filename);

            genre = TStringToQString(tag->genre());
            year = tag->year();

            if (year > 0)
            {
                year_text = QString::number(year);
            }
            else
            {
                year_text = "";
            }

            length = f.audioProperties()->lengthInSeconds();

            hour = length / 3600;
            length = length % 3600;
            minute = length / 60;
            length = length % 60;
            sec = length;

            duration_time.setHMS(hour, minute, sec);

            if (hour == 0)
                duration_text = duration_time.toString("m:ss");
            else
                duration_text = duration_time.toString("h:mm:ss");

            songList["Title"].insert(i, title);
            songList["Artist"].insert(i, artist);
            songList["Album"].insert(i, album);
            songList["Genre"].insert(i, genre);
            songList["Year"].insert(i, year_text);
            songList["Duration"].insert(i, duration_text);
        }

        emit createdSongList(songList);
    }
        break;
    }
}

