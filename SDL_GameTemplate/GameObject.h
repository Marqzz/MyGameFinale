#pragma once
#include "SDL_image.h"

class GameObject
{
public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:
	int _x;
	int _y;
	SDL_Texture* _texture;
	SDL_Rect _srcRectangle;
	SDL_Rect _destRectangle;
	
};