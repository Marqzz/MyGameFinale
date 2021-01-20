#pragma once
/// <summary>
/// Base class for each component 
/// </summary>


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//Entity–component–system (ECS) is an architectural pattern that is mostly used in game development.
//ECS follows the composition over inheritance principle that allows greater flexibility in defining entities where every object in a game's scene is an entity (e.g. enemies, bullets, vehicles, etc.). 
//Every entity consists of one or more components which contains data or state. 
//Therefore, the behavior of an entity can be changed at runtime by systems that add, remove or mutate components. 
//This eliminates the ambiguity problems of deep and wide inheritance hierarchies that are difficult to understand, maintain and extend. 
//Common ECS approaches are highly compatible and often combined with data-oriented design techniques.


class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;
//using groups for a better categorizare

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;//static:to have unique ID's
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
//basically seeing if an entity has or no a specified component
//that's why we have bitset bcs it only stores 0/1
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;


class Component
{
public:
	Entity* entity;

   virtual ~Component() = default;
   virtual void init() {}
   virtual void update() {}
   virtual void draw() {}
   //am schimbat aici pentru ca atunci cand am implementat ECS , spunea ca o clasa abstracta nu poate fi instantiata , asadar , nu mai este abstracta , si folosim override
};

class Entity
{
private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;
	//a vector to store the components of our Entity

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	Entity(Manager& mManager) : manager(mManager) {}

	void init()
	{
		for (auto& c : components) c->init();
	}

	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw() 
	{
		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}
	
	template <typename T, typename... TArgs>T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));//kind of "?"
		c->entity = this;

		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		//Construct and insert element at the end

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}
	
	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	// a list of all entities
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
	void update()
	{
		for (auto& e : entities) e->update();
	}
	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()//??-remove entities if they are not active
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
						std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}
	
	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
		//this will give us the list of the entities inside of a group
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e }; //size = e
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};