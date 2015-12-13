#include "manager.h"
#include <QStringList>

Master Manager::master;

Manager::Manager(QObject *parent) :
    QObject(parent)
{

}

Manager::~Manager()
{

}


void Manager::split(const std::string& s, char c, std::vector<std::string>& v)
{
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);

   while (j != std::string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == std::string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

void Manager::GetFileListing(std::string directory, std::string fileFilter, Master &list, bool recursively)
{
    if (recursively)
        GetFileListing(directory, fileFilter, list, false);

    directory += "/";

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::string filter = directory + (recursively ? "*" : fileFilter);

    WCHAR wstr[260];
    MultiByteToWideChar( 0,0, filter.c_str(), 260, wstr, 260);
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
            Song temp;

            std::wstring wstr(FindFileData.cFileName);

            std::string str( wstr.begin(), wstr.end() );

            temp.SetPath(directory + str);
            list.AddToList(temp);
            //std::cout << directory + std::string(FindFileData.cFileName) << std::endl;
        }

        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (!recursively)
            {
                Song temp;

                std::wstring wstr(FindFileData.cFileName);

                std::string str( wstr.begin(), wstr.end() );

                temp.SetPath(directory + str);
                list.AddToList(temp);
                //std::cout << directory + std::string(FindFileData.cFileName) << std::endl;
            }
            else
            {
                if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)>0 && FindFileData.cFileName[0]!='.')
                {
                    std::wstring wstr(FindFileData.cFileName);

                    std::string str( wstr.begin(), wstr.end() );

                    GetFileListing(directory + str, fileFilter, list);
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

void Manager::GetFiles(std::string directory, std::string fileFilter, Master &list, bool recursively)
{
    std::vector<std::string> ex;
    split(fileFilter, '|', ex);
    for (int i = 0; i < ex.size(); i++)
    {
        GetFileListing(directory, ex[i], list, recursively);
    }
}

void Manager::CreateMaster(QStringList str_list)
{
    for (int i = 0; i < str_list.size(); i++)
    {
        GetFiles(str_list.at(i).toUtf8().constData(), "*.mp3|*.wav", master, true);
    }
}
