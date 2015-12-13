#ifndef SONG
#define SONG

class Song
{
private:
	std::string _id;
	std::string _path;
public:
	Song(void);
	~Song(void);
	Song(std::string id, std::string path);

	void SetID(std::string id);
	void SetPath(std::string path);

	std::string GetID() const;
	std::string GetPath() const;
};

#endif //SONG
