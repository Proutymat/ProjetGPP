#pragma once

#include <vector>
#include <SFML/Window/Joystick.hpp>
#include "sys.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Map.hpp"



using namespace sf;

class HotReloadShader;
class Game {
public:

	/* ATTRIBUTES */

	int screenSizeX = 1280;
	int screenSizeY = 720;
	
	float gameTime = 0.0;
	double tickTimer = 0.0;
	float gravity = 144.0f;

	float fireRate = 0.075f;
	float fireRateTimer = 0;
	float lastFireRateTimeUpdate = 0;

	std::mt19937 seed; /**< The random number generator */
	std::uniform_real_distribution<float> rand_float{-10, 10}; /**< The random float generator */
	
	
	Camera camera;
	Map map;
	
	sf::RenderWindow* win = nullptr;

	sf::RectangleShape bg;
	HotReloadShader * bgShader = nullptr;

	sf::Texture	tex;

	bool closing = false;
	
	std::vector<Entity> entities;
	std::vector<Bullet> bullets;

	Entity player;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Game(sf::RenderWindow * win);
	
	void processInput(sf::Event ev);
	bool wasPressed = false;
	void handleKeyboardEvents(double deltaTime);
	void handleEnemiesMovement();

	void update(double deltaTime);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	bool PlayerCollideWithEnemy();
	void im();
};