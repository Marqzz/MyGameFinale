#pragma once
#include <string>
#include "SDL.h"
#include "components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string t);
	//{
	//	tag = t;
	//}

	ColliderComponent(std::string t, int xpos, int ypos);
	//{
		//tag = t;
		//collider.x = xpos;
		//collider.y = ypos;
		//collider.h = collider.w = 32;
	//}

	void init()override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/coltex.png");
		srcRect = { 0,0,32,32 };
		destRect = { collider.x,collider.y,collider.w,collider.h };

	}

	void update()override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
	}

	void draw()override
	{
		TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
	}
};