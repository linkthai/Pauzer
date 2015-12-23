#include "Master.h"


Master::Master(void)
{
    _id = L"";
    _name = L"";
	_songCount = 0;
}


Master::~Master(void)
{
}

void Master::AddToList(Song song)
{
    song.SetID(std::to_wstring(_songCount));
	_list.push_back(song);
	_songCount++;
}

void Master::RemoveFromList(std::wstring id)
{
    for (unsigned int i = 0; i < _list.size(); i++)
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

Song Master::Get(std::wstring id)
{
    for (unsigned int i = 0; i < _list.size(); i++)
		if (_list[i].GetID() == id)
		{
			return _list[i];
		}
}
void Master::SetName(std::wstring name)
{
	_name = name;
}

std::wstring Master::GetName()
{
	return _name;
}

int Master::GetCount()
{
	return _songCount;
}

void Master::ClearList()
{
    _songCount = 0;
    _list.clear();
}
