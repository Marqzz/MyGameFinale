#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Timer.h"

class AssetManager;
//class ColliderComponent - aici a fost o eroare - clasa era declarata altundeva decat unde era nevoie

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update() const;
	void render() const;
	void clean() const;

	bool running() const { return isRunning;  }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static AssetManager* assets;
	static LTimer timer;

	enum groupLabels : std::size_t
	{
		//now we can name some of our groups;max 32
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles
	};

private:
	SDL_Window* window;
	
};
