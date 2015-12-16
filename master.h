#ifndef MASTER
#define MASTER

#include "song.h"
#include <vector>
#include <string>

class Master
{
private:
    std::wstring _id;
    std::wstring _name;
	int _songCount;
	std::vector<Song> _list;
public:
	Master(void);
	~Master(void);
	void AddToList(Song song);
    void RemoveFromList(std::wstring id);
	void RemoveFromList(int pos);
	Song Get(int pos);
    Song Get(std::wstring id);
    void SetName(std::wstring name);
    std::wstring GetName();
	int GetCount();
};

#endif // MASTER

