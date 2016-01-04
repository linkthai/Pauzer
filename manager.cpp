#include "manager.h"
#include <QStringList>

Master Manager::master;
Xml_Parser Manager::parser;
Setting Manager::setting;

Manager::Manager(QObject *parent) :
    QObject(parent)
{

}

Manager::~Manager()
{

}

bool Manager::fileExists(QString path)
{
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Manager::split(const std::wstring& s, char c, std::vector<std::wstring>& v)
{
   std::wstring::size_type i = 0;
   std::wstring::size_type j = s.find(c);

   while (j != std::wstring::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == std::wstring::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

void Manager::GetFileListing(std::wstring directory, std::wstring fileFilter, Xml_Parser &parser, bool recursively)
{
    if (recursively)
        GetFileListing(directory, fileFilter, parser, false);

    directory += L"/";

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::wstring filter = directory + (recursively ? L"*" : fileFilter);

    const wchar_t * wstr;
    wstr = filter.c_str();
    LPCWSTR filter_lpcwstr = wstr;

    hFind = FindFirstFile(filter_lpcwstr, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }
    else
    {
        if (!recursively)
        {
            std::wstring wstr(FindFileData.cFileName);

            std::wstring str( wstr.begin(), wstr.end() );

            QString title, artist, album, filename;
            int duration;
            uint year;
            filename = QString::fromStdWString(directory + str);

            TagLib::MPEG::File f( reinterpret_cast<const wchar_t*>(filename.constData()) );
            TagLib::ID3v2::Tag *tag = f.ID3v2Tag();

            TagLib::ID3v2::FrameList albart = tag->frameList("TPE2");
            if (!albart.isEmpty())
            {
                artist = TStringToQString(albart.front()->toString());
            }
            else
                artist = TStringToQString(f.tag()->artist());

            artist = artist.simplified();
            title = TStringToQString(f.tag()->title());
            album = TStringToQString(f.tag()->album());
            year = f.tag()->year();
            duration = f.audioProperties()->lengthInSeconds();

            CheckSongInfo(title, artist, album, filename);
            parser.AddToDom(title, artist, album, filename, year);
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (!recursively)
            {
                std::wstring wstr(FindFileData.cFileName);

                std::wstring str( wstr.begin(), wstr.end() );

                QString title, artist, album, filename;
                uint year;
                int duration;
                filename = QString::fromStdWString(directory + str);

                TagLib::MPEG::File f( reinterpret_cast<const wchar_t*>(filename.constData()) );

                title = TStringToQString(f.tag()->title());
                artist = TStringToQString(f.tag()->artist());
                album = TStringToQString(f.tag()->album());
                year = f.tag()->year();
                duration = f.audioProperties()->lengthInSeconds();

                CheckSongInfo(title, artist, album, filename);
                parser.AddToDom(title, artist, album, filename, year);
            }
            else
            {
                if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)>0 && FindFileData.cFileName[0]!='.')
                {
                    std::wstring wstr(FindFileData.cFileName);

                    std::wstring str( wstr.begin(), wstr.end() );

                    GetFileListing(directory + str, fileFilter, parser);
                }
            }
        }

        DWORD dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES)
        {
            std::cout << "FindNextFile error. Error is "<< dwError << std::endl;
        }
    }
}

void Manager::GetFiles(std::wstring directory, std::wstring fileFilter, Xml_Parser &parser, bool recursively)
{
    std::vector<std::wstring> ex;
    split(fileFilter, '|', ex);

    for (unsigned int i = 0; i < ex.size(); i++)
    {
        GetFileListing(directory, ex[i], parser, recursively);
    }
}

void Manager::CreationProcessSuccess(bool isSuccessful)
{
    if (isSuccessful)
    {
        if (!QDir("Data").exists())
            QDir().mkdir("Data");
        parser.SaveChoosenFolder("Data\\Master.xml");
    }
}

void Manager::CreateMaster(QStringList str_list)
{
    if (master.GetCount() != 0)
        master.ClearList();

    parser.Init();

    for (int i = 0; i < str_list.size(); i++)
    {
        GetFiles(str_list.at(i).toStdWString(), L"*.mp3|*.wav", parser, true);
    }
    master.SetList(parser.GetAllSong());
    master.SetAlbumList(parser.GetAllAlbums());
}

void Manager::CheckSongInfo(QString &title, QString &artist, QString &album, const QString &path)
{
    if (title == "")
    {
        QStringList name = path.split("/");
        title = name.value(name.length() - 1);
        name = title.split(".");
        title = name.value(0);
    }

    if (album == "")
    {
        album = "Unknown";
    }

    if (artist == "")
    {
        artist = "Unknown";
    }
}

bool Manager::LoadSongToMaster()
{
    QString XmlPath = "Data\\Master.xml";
    if (fileExists(XmlPath))
    {
        parser.LoadData(XmlPath);
        parser.GetSongsInPlaylist(master);
        parser.GetAlbumsInPlaylist(master);
        return true;
    }
    return false;
}
