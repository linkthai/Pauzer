#ifndef MANAGER
#define MANAGER

#include <WinBase.h>
#include <Windows.h>

#include "master.h"
#include "xml_parser.h"
#include "song.h"

class Manager : public QObject
{
    Q_OBJECT
private:
    void static split(const std::wstring& s, char c, std::vector<std::wstring>& v);
    void static GetFileListing(std::wstring directory, std::wstring fileFilter, Xml_Parser &parser, bool recursively = true);
    void static GetFiles(std::wstring directory, std::wstring fileFilter, Xml_Parser &parser, bool recursively = true);
public:
    static Master master;
    static Xml_Parser parser;
    static void CreationProcessSuccess(bool isSuccessful);
    static void CreateMaster(QStringList str_list);
    static void CheckSongInfo(QString &title, QString &artist, QString &album, const QString &path);
    static bool LoadSongToMaster();
    static bool fileExists(QString path);
    explicit Manager(QObject *parent = 0);

    ~Manager();
};

#endif // MANAGER

