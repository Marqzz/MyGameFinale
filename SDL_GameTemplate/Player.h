#pragma once
#include "ECS.h"
#include "SDL.h"

class Player : public Component
{
	SDL_Texture* texture{};
	SDL_Renderer* renderer{};
	SDL_Rect srcRect{}, destRect{};

public:
   Player() = default;
   Player(char* path, SDL_Renderer* renderer);

	void setTex( char* path);

	void init() override;

	void update() override;

	void draw() override;

	void upLevel()
	{
		srcRect.x = srcRect.y = 0;
	}
};
