#pragma once
#include <SDL.h>
#include <iostream>
#include "Map.h"
#include "TextureManager.h"

class Button : public Component
{
public:
	Button(int xpos,int ypos,const char* path,Uint32 butId)
	{
		Position.x = xpos;
		Position.y = ypos;
		Position.w = Position.h = 32;
		id = butId;
		map = new Map("terrain");

		texture = TextureManager::LoadTexture(path);
	}
	void setPosition();
	void render()
	{
		SDL_RenderCopy(Game::renderer, texture, nullptr, &Position);
	}
	void handleEvent(SDL_Event* e)
	{
		if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			//if we click , we have to see if we clicked inside the button or no
			SDL_GetMouseState(&x, &y);
			bool click = true;
			//mouse is left of the button
			if (x < Position.x)
				click = false;
			//mouse is right of the button
			if (x > Position.x + 32)
					click = false;
			//mouse above the button
			if (y < Position.y)
					click = false;
			//mouse below the button
			if (y > Position.y + 32)
					click = false;
			if (click == true)
				//	change.map();
			{
				this->Action(*this->map);
				std::cout << "Ai atins butonul " << this->id<< std::endl;
			}
		}
	}
	Button()
	{

	}
	virtual void Action(Map& map) = 0;//aici schimb hartile boss -- / luam harta ca parametru
private:
	SDL_Rect Position;
	SDL_Texture* texture;
	Uint32 id;
	Map* map;
};

//another solution would be if we make an array that stores ID's of the map 
class Button1 : public Button
{
public:
	void Action(Map& map)
	{
		map.LoadMap("assets/map1.map", 25, 20,true);
	}
	Button1(int xpos, int ypos, const char* path, Uint32 butId) :Button(xpos, ypos, path, butId)
	{

	}
};
class Button2 : public Button
{
public:
	void Action(Map& map)
	{
		//SDL_DestroyRenderer(Game::renderer);
		//SDL_RenderClear(Game::renderer);
		//player.addComponent<TransformComponent>(0, 0, 32, 32, 5);
		map.LoadMap("assets/map2.map", 25, 20,true);
	}
	Button2(int xpos, int ypos, const char* path, Uint32 butId) :Button(xpos, ypos, path, butId)
	{

	}
};
class Button3 : public Button
{
public:
	void Action(Map& map)
	{
		map.LoadMap("assets/map3.map", 25, 20,true);
	}
	Button3(int xpos, int ypos, const char* path, Uint32 butId) :Button(xpos, ypos, path, butId)
	{

	}
};