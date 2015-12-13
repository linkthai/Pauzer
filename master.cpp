#include "Master.h"


Master::Master(void)
{
	_id = "";
	_name = "";
	_songCount = 0;
}


Master::~Master(void)
{
}

void Master::AddToList(Song song)
{
	song.SetID(std::to_string(_songCount));
	_list.push_back(song);
	_songCount++;
}

void Master::RemoveFromList(std::string id)
{
	for (int i = 0; i < _list.size(); i++)
		if (_list[i].GetID() == id)
		{
			_list.erase(_list.begin() + i);
			_songCount--;
		}
}

void Master::RemoveFromList(int pos)
{
	_list.erase(_list.begin() + pos);
	_songCount--;
}

Song Master::Get(int pos)
{
	return _list[pos];
}

Song Master::Get(std::string id)
{
	for (int i = 0; i < _list.size(); i++)
		if (_list[i].GetID() == id)
		{
			return _list[i];
		}
}
void Master::SetName(std::string name)
{
	_name = name;
}

std::string Master::GetName()
{
	return _name;
}

int Master::GetCount()
{
	return _songCount;
}
