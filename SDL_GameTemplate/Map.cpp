#include "Map.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include "ECS.h"
#include "TileComponent.h"
#include "ColliderComponent.h"

extern Manager manager;

Map::Map(std::string tID) : texID(tID)
{
}

Map::~Map()
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY,bool destroy)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX;
	int srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * 32;

			mapFile.get(c);
			srcX = atoi(&c) * 32;
			
			AddTile(srcX, srcY, x * 32, y * 32);
			mapFile.ignore();
		}
	}

	mapFile.ignore();
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{	
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * 32, y * 32);
				tcol.addGroup(Game::groupColliders);
				if (destroy == true)
					tcol.destroy();
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos,texID);
	tile.addGroup(Game::groupMap);
}

