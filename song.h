#ifndef SONG
#define SONG

class Song
{
private:
    std::wstring _id;
    std::wstring _path;
public:
	Song(void);
	~Song(void);
    Song(std::wstring id, std::wstring path);

    void SetID(std::wstring id);
    void SetPath(std::wstring path);

    std::wstring GetID() const;
    std::wstring GetPath() const;
};

#endif //SONG
