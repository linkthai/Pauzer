#ifndef MASTER
#define MASTER

#include "song.h"
#include <vector>
#include <string>

class Master
{
private:
	std::string _id;
	std::string _name;
	int _songCount;
	std::vector<Song> _list;
public:
	Master(void);
	~Master(void);
	void AddToList(Song song);
	void RemoveFromList(std::string id);
	void RemoveFromList(int pos);
	Song Get(int pos);
	Song Get(std::string id);
	void SetName(std::string name);
	std::string GetName();
	int GetCount();
};

#endif // MASTER

