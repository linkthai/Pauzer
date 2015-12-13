#ifndef MANAGER
#define MANAGER

#include <WinBase.h>
#include <Windows.h>

#include "master.h"
#include "song.h"

class Manager : public QObject
{
    Q_OBJECT
private:
    void static split(const std::string& s, char c, std::vector<std::string>& v);
    void static GetFileListing(std::string directory, std::string fileFilter, Master &list, bool recursively = true);
    void static GetFiles(std::string directory, std::string fileFilter, Master &list, bool recursively = true);
public:
    static Master master;
    static void CreateMaster(QStringList str_list);
    explicit Manager(QObject *parent = 0);
    ~Manager();
};

#endif // MANAGER

