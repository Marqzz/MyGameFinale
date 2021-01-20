#include "Game.h"
#include <iostream>
#include "Map.h"
#include "TextureManager.h"
#include "components.h"
#include "Player.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "Button.h"
#include "Life.h"
#include <ctime>
#include <sstream>

bool lvlUp = false;

static int cnt = 0;

Button1* b1;
Button2* b2;
Button3* b3;

Life LIFE;

Map* map;
Manager manager;

LTimer Game::timer;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& label(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	timer.start();
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized" << std::endl;
		auto window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window!=nullptr)
		{
			std::cout << "Window created" << std::endl;
		}
 
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (renderer != nullptr)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);
			std::cout << "Renderer created" << std::endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "TTF Failed" << std::endl;
	}
	//ECS implementation

	srand((unsigned)time(0));//for random projectiles

	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/player_anims.png");
	assets->AddTexture("projectile", "assets/proj.png");

	assets->AddFont("arial", "assets/arial.ttf", 16);

	assets->AddTexture("button1", "assets/lvl1buttonPNG.png");
	assets->AddTexture("button2", "assets/lvl2buttonPNG.png");
	assets->AddTexture("button3", "assets/lvl3buttonPNG.png");

	//lvl2->lvl3 (678,510)-- maybe with an SDL_Rect ; like if(AABB(player,SDL_Rect)==true)->move map
	//678,447

	map = new Map("terrain");
	map->LoadMap("assets/map1.map", 25, 20,false);

	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>("player",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	b1 = new Button1(10, 70, "assets/lvl1buttonPNG.png", 1);
	b2 = new Button2(10, 105, "assets/lvl2buttonPNG.png", 2);
	b3 = new Button3(10, 140, "assets/lvl3buttonPNG.png", 3);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "TEST", "arial", white);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT: {
		isRunning = false;
		break;
	}
	default:
		break;
	}
	b1->handleEvent(&event);
	b2->handleEvent(&event);
	b3->handleEvent(&event);
}
void Game::update()const 
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	std::cout << "(" << playerPos.x << "," << playerPos.y << ")" << std::endl;

	std::stringstream ss;//this is a variable who holds OUTPUT streams ; like COUT
	ss << "Time " << Game::timer.getTicks() / 1000 << ":" << Game::timer.getTicks() % 1000 << "   Lifes:" << LIFE.getLifes();
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		//checking players collider into map collider
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			//forcing back the player to the last position
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Player hit by projectile !" << std::endl;
			p->destroy();//if the player was hit , we destroy the collider
			LIFE.minusLife();
		}
	}

	cnt++;
	if (cnt % 225 == 0)
	{
		assets->CreateProjectile(Vector2D(800, rand()%500), Vector2D(-1, 0), 400, 1, "projectile");
		assets->CreateProjectile(Vector2D(0, rand() % 500), Vector2D(1, 0), 400, 1, "projectile");
		assets->CreateProjectile(Vector2D(rand() % 500, 240), Vector2D(1, 0), 400, 1, "projectile");
	}

	if (LIFE.getLifes() == 0)
	{
		std::cout << "GAME OVER" << std::endl;
		Game::isRunning = false;
	}
	
	if ((playerPos.x == 10 && playerPos.y == 497) || (playerPos.x == 0 && playerPos.y == 480) || (playerPos.x == 0 && playerPos.y == 489) || (playerPos.y == 486 && playerPos.x == 0))
	{
		lvlUp = true;

		b2->Action(*map);
		//playerPos.x = playerPos.y = 0;
	}
}

void Game::render() const
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	//this is where we ACTUALLY draw the colliders
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		if (lvlUp == true)
		{
			p->init();
			p->draw();
			lvlUp = false;
		}
		else
			p->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}

	label.draw();

	b1->render();
	b2->render();
	b3->render();

	SDL_RenderPresent(renderer);
}

void Game::clean() const
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit;
	SDL_QUIT;
	std::cout << "Game cleaned" << std::endl;
}

