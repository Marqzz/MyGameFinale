#pragma once
#include <string>

class Map
{
public:
	Map(std::string tID);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY,bool destroy);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
	std::string texID;
};