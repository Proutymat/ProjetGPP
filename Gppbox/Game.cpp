
#include <imgui.h>
#include <array>
#include <vector>
#include "C.hpp"
#include "Game.hpp"
#include "HotReloadShader.hpp"

Game::Game(sf::RenderWindow * win) : camera(this), map() {
	// Initialize SFML window
	this->win = win;
	
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	// Load background texture
	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(screenSizeX, screenSizeY));

	// Load background shader
	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	

	// Player initialization
	player = Entity(this, 5, 23, false);

	camera.x = player.xx;
	camera.y = player.yy;
}



void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}

	// Left mouse click : add wall
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		int cx = (camera.mouseX - screenSizeX / 2) / C::GRID_SIZE;
		int cy = (camera.mouseY - screenSizeY / 2) / C::GRID_SIZE;
		if (!isWall(cx, cy)) {
			map.addWall(cx, cy);
		}
	}
}



void Game::handleKeyboardEvents(double deltaTime) {
	player.moveX = 0;
	
	// Left direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		player.moveX += -20;
		player.direction = -1;
	}
	else if (Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) < -20) {
        player.moveX = Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) * 0.25F;
		player.direction = -1;
	}

	// Right direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.moveX += 20;
		player.direction = 1;
	}
	else if (Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) > 20) {
        player.moveX = Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) * 0.25F;
		player.direction = 1;
    }

	// Jump
	if (((Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) || Joystick::isButtonPressed(0, 1)) && player.onGround) {
		player.moveY = -40;
	}
	else if ((Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) || Joystick::isButtonPressed(0, 1)) {
		if (!wasPressed) {
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

	// Shooting
	fireRateTimer += gameTime - lastFireRateTimeUpdate;
	lastFireRateTimeUpdate = gameTime;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) || Joystick::isButtonPressed(0, 7)) {
		player.moveX += -10 * player.direction;
		camera.setShake(0.05, 2);
		if (fireRateTimer >= fireRate)
		{
			fireRateTimer = 0;
			bullets.emplace_back(player.direction, player.xx, player.yy + rand_float(seed), gameTime);
		}
	}

	
}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::handleEnemiesMovement()
{
	for (int i = 0; i < entities.size(); i++){
		entities[i].moveX = 10 * entities[i].direction;
	}
}

void Game::update(double deltaTime) {
	gameTime += static_cast<float>(deltaTime);
	// Handle inputs
	handleKeyboardEvents(deltaTime);

	// Handle enemies movement
	handleEnemiesMovement();
	
	if (bgShader) bgShader->update(deltaTime);
	beforeParts.update(deltaTime);

	player.applyMovement(deltaTime);
	
	// Apply entities movement
	for (Entity& entity : entities)
	{
		entity.applyMovement(deltaTime);
	}


	// Update bullets
	int i = 0;
	std::vector<int> bulletsToDelete;
	std::vector<int> enemiesToDelete;
	for (Bullet& bullet : bullets)
	{
		// Update bullets, destroy them if life time is over or if they hit a wall
		if (bullet.update(deltaTime, map.walls, gameTime))
		{
			bulletsToDelete.emplace_back(i);
		}
		else
		{
			int k = 0;
			for (Entity& entity : entities)
			{
				if (bullet.collideWith(entity))
				{
					bulletsToDelete.emplace_back(i);
					enemiesToDelete.emplace_back(k);
				}
			}
		}
		
		i++;
	}	

	// Destroy bullets and enemies
	for (int indice : bulletsToDelete)
	{
		bullets.erase(bullets.begin() + indice);
	}
	for (int indice : enemiesToDelete)
	{
		entities.erase(entities.begin() + indice);
	}
	bulletsToDelete.clear();
	enemiesToDelete.clear();

	PlayerCollideWithEnemy();

	camera.update(static_cast<float>(deltaTime));
	afterParts.update(deltaTime);
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", gameTime);
	win.draw(bg, states);

	beforeParts.draw(win);

	// Draw walls
	for (sf::RectangleShape & r : map.wallSprites)
		win.draw(r);

	player.draw(win);
	
	// Draw entities
	for (Entity& entity : entities)
		entity.draw(win);

	// Draw others
	for (sf::RectangleShape& r : rects) 
		win.draw(r);

	// Draw bullets
	for (Bullet& bullet : bullets)
		bullet.draw(win);

	afterParts.draw(win);
}

bool Game::isWall(int cx, int cy)
{
	// Check each wall
	for (Vector2i & wall : map.walls) {
		if (wall.x == cx && wall.y == cy)
			return true;
	}
	return false;
}

bool Game::PlayerCollideWithEnemy()
{
	for (Entity& entity : entities)
    {
        if (player.collideWith(entity))
        {
        	player.setPositions(75, 0);
        	return true;
        }
        	
    }
    return false;	
}

void Game::im()
{
	map.imgui();
	camera.imgui();

	// Player header
	if (ImGui::CollapsingHeader("Player"))
	{
		ImGui::Value("NbBullets", (int)bullets.size());
		bool edit = false;
		float xx = player.xx;
		float yy = player.yy;
		edit |= ImGui::DragFloat("player xx", &xx, 0.1f);
		edit |= ImGui::DragFloat("player yy", &yy, 0.1f);
		if (edit) {
			player.setPositions(xx, yy);
		}

		ImGui::DragFloat("player dx", &player.moveX, 0.1f);
		ImGui::DragFloat("player dy", &player.moveY, 0.1f);
	}

	// Entities header
	if (ImGui::CollapsingHeader("Entities"))
	{
		ImGui::Text("Number of entities : %d", entities.size());
		if (ImGui::Button("Add new enemy")) {
			entities.emplace_back(this, 50, -100, true);
			entities.back().direction = 1;
		}
	}
}
