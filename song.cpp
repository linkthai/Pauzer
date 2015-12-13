#include "Song.h"

Song::Song(void)
{
	_id = "";
	_path = "";
}

Song::~Song(void)
{
}

Song::Song(std::string id, std::string path)
{
	_id = id;
	_path = path;
}

void Song::SetID(std::string id) 
{ 
	_id = id; 
}
void Song::SetPath(std::string path)
{
	_path = path;
}

std::string Song::GetID() const
{
	return _id;
}
std::string Song::GetPath() const
{
	return _path;
}
