
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
	entities.emplace_back(this, 5, 23);
	player = &entities.back();

	camera.x = player->xx;
	camera.y = player->yy;
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
		int cx = (camera.mouseX) / C::GRID_SIZE;
		int cy = (camera.mouseY - screenSizeY / 2) / C::GRID_SIZE;
		if (!isWall(cx, cy)) {
			map.addWall(cx, cy);
		}
	}
}



void Game::handleKeyboardEvents(double deltaTime) {
	
	// Left direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		player->moveX = -20;
	}
	else if (Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) < -20) {
        player->moveX = Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) * 0.25F;
		
	}

	// Right direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player->moveX = 20;
	}
	else if (Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) > 20) {
        player->moveX = Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::X) * 0.25F;
    }
	printf("JOYSTICK AXIS = %f\n", Joystick::getAxisPosition(sf::Joystick::X, sf::Joystick::Axis::Y));

	// Jump
	if (((Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) || Joystick::isButtonPressed(0, 1)) && player->onGround) {
		player->moveY = -40;
	}
	else if ((Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) || Joystick::isButtonPressed(0, 1)) {
		if (!wasPressed) {
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
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
	for (int i = 1; i < entities.size(); i++){
		entities[i].moveX += 10 * entities[i].direction;
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

	// Apply entities movement
	for (Entity& entity : entities)
	{
		entity.applyMovement(deltaTime);
	}

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

	// Draw entities
	for (Entity& e : entities)
		e.draw(win);

	// Draw others
	for (sf::RectangleShape& r : rects) 
		win.draw(r);

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

void Game::im()
{
	map.imgui();
	camera.imgui();

	// Player header
	if (ImGui::CollapsingHeader("Player"))
	{
		bool edit = false;
		float xx = player->xx;
		float yy = player->yy;
		edit |= ImGui::DragFloat("player xx", &xx, 0.1f);
		edit |= ImGui::DragFloat("player yy", &yy, 0.1f);
		if (edit) {
			player->setPositions(xx, yy);
		}

		ImGui::DragFloat("player dx", &player->moveX, 0.1f);
		ImGui::DragFloat("player dy", &player->moveY, 0.1f);
	}

	// Entities header
	if (ImGui::CollapsingHeader("Entities"))
	{
		ImGui::Text("Number of entities : %d", entities.size());
		if (ImGui::Button("Add new enemy")) {
			entities.emplace_back(this, 50, -100);
		}
	}
}
