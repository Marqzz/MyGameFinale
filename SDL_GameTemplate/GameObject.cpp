#include "GameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include <SDL.h>

GameObject::GameObject(const char* textureSheet, int x, int y)
{
   _texture = TextureManager::LoadTexture(textureSheet);
   _x = x;
   _y = y;
}

GameObject::~GameObject() = default;

void GameObject::Update() {
	_x++;
	_y++;
	_srcRectangle.h = 32;
	_srcRectangle.w = 32;
	_srcRectangle.x = 0;
	_srcRectangle.y = 0;

	_destRectangle.x = _x;
	_destRectangle.y = _y;
	_destRectangle.w = 32;
	_destRectangle.h = 32;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer , _texture, nullptr, &_destRectangle);
}