#include "Player.h"
#include "TextureManager.h"

Player::Player( char* path, SDL_Renderer* renderer): renderer(renderer)
{
   setTex(path);
}

void Player::setTex( char* path)
{
   texture = TextureManager::LoadTexture(path);
}


void Player::init()
{
   srcRect.x = srcRect.y = 0;
   srcRect.w = srcRect.h = 32;
   destRect.w = destRect.h = 64;
}

void Player::update()
{
   destRect.x++;
   destRect.y++;
}


void Player::draw()
{
   TextureManager::Draw(texture, srcRect, destRect,SDL_FLIP_NONE);
}