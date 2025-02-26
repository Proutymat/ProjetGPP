#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "sys.hpp"
#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "Entity.hpp"
#include "Camera.hpp"

using namespace sf;

class HotReloadShader;
class Game {
public:

	/* ATTRIBUTES */
	
	float gameTime = 0.0;
	double tickTimer = 0.0;
	float gravity = 144.0f;
	Camera camera = Camera(this);
	
	sf::RenderWindow* win = nullptr;

	sf::RectangleShape bg;
	HotReloadShader * bgShader = nullptr;

	sf::Texture	tex;

	bool closing = false;
	
	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallSprites;
	std::vector<Entity> entities;

	Entity* player = nullptr;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Game(sf::RenderWindow * win);

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void handleKeyboardEvents(double deltaTime);
	void onSpacePressed();

	void update(double deltaTime);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();
};