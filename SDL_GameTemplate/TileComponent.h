#pragma once

#include "ECS.h"
#include "SDL.h"
#include "Vector2D.h"
#include "TextureManager.h"
#include "AssetManager.h"

class TileComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX,int srcY,int xpos,int ypos,std::string id)
	{
		//could change the scale of our tiles by adding extra parameters to this constructor
		//but I don t have to , bcs of the logic of the game
		
		texture = Game::assets->GetTexture(id);

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = 32;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = 32;
	}
	
	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

};