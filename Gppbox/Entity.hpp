#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game;

class Entity {
public:

    /* ATTRIBUTES */
    
    // MOVEMENT
    int cx, cy; // Grid coordinates
    float xr, yr; // Ratios
    float xx, yy; // Resulting coordinates
    float moveX, moveY; // Movement

    int direction = 1; // -1 left, 0 none, 1 right

    sf::RectangleShape sprite;
    bool onGround = false;

    
    /* CONSTRUCTORS */

    Entity();
    Entity(Game* game, int x, int y, bool isEnemy);

    
    /* METHODS */

    void setPositions(float x, float y);
    void applyMovement(double detlaTime);
    bool collideWith(Entity& other);
    void draw(sf::RenderWindow& win) const;

private:
    Game* game;
};