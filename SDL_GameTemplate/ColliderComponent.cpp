#include "ColliderComponent.h"


ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos)
{
	tag = t;
	collider.x = xpos;
	collider.y = ypos;
	collider.h = collider.w = 32;
}

ColliderComponent::ColliderComponent(std::string t)
{
	tag = t;
}