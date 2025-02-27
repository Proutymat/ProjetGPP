#pragma once

#include <vector>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "C.hpp"

class Map
{
public:

    /* ATTRIBUTES */

    int cols = 1280 / C::GRID_SIZE;
    int lastLine = 720 / C::GRID_SIZE - 1;
    
    std::vector<sf::Vector2i> walls;
    std::vector<sf::RectangleShape> wallSprites;


    /* CONSTRUCTORS */

    Map();


    /* METHODS */

    void cacheWalls();
};
