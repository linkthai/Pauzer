#include "Song.h"

Song::Song(void)
{
    _id = L"";
    _path = L"";
}

Song::~Song(void)
{
}

Song::Song(std::wstring id, std::wstring path)
{
	_id = id;
	_path = path;
}

void Song::SetID(std::wstring id)
{ 
	_id = id; 
}
void Song::SetPath(std::wstring path)
{
	_path = path;
}

std::wstring Song::GetID() const
{
	return _id;
}
std::wstring Song::GetPath() const
{
	return _path;
}
