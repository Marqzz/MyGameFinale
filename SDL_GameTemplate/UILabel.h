#pragma once
#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:
	UILabel(int xpos,int ypos,std::string text,std::string font,SDL_Color& colour):
		labelText(text),labelFont(font),textColor(colour)
	{
		position.x = xpos;
		position.y = ypos;

		SetLabelText(labelText,labelFont);
	}
	~UILabel() {}

	void SetLabelText(std::string text,std::string font)
	{
		//creating an image of our text
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(labelFont), text.c_str(), textColor);

		//turn the surface into a texture that we can render
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);

		//free the surface
		SDL_FreeSurface(surf);

		//now getting width and heigh of our texture that has been created
		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw()override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);//that will draw our texture to the screen
	}

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};