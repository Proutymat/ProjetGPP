#pragma once

#include "C.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "C.hpp"
#include <imgui.h>
#include "Entity.hpp"


class Bullet
{
public:
    // MOVEMENT
    int cx, cy; // Grid coordinates
    float xr, yr; // Ratios
    float xx, yy; // Resulting coordinates
    float moveX, moveY; // Movement
    int direction = 0; // -1 left, 0 none, 1 right

    float lifeTime = 3.0f;
    float timePassed = 0.0f;
    float lastTimeUpdate = 0.0f;

    sf::RectangleShape sprite;

    /* CONSTRUCTORS */

    Bullet(int direction, int x, int y, float gameTime);

    bool collideWith(Entity& other);
    void setPositions(float x, float y);
    bool isWall(std::vector<sf::Vector2i> walls, int cx, int cy);
    bool update(float deltaTime, std::vector<sf::Vector2i> walls, float gameTime);
    void draw(sf::RenderWindow& win) const;

};
