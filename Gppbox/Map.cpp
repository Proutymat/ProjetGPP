#include "Map.hpp"



/* CONSTRUCTORS */

Map::Map()
{
    // Add ground walls
    for (int i = 0; i < 1280 / C::GRID_SIZE; ++i) 
        walls.push_back(sf::Vector2i(i, lastLine) );

    // Add borders and obstacles walls 
    walls.push_back(sf::Vector2i(0, lastLine-1));
    walls.push_back(sf::Vector2i(0, lastLine-2));
    walls.push_back(sf::Vector2i(0, lastLine-3));

    walls.push_back(sf::Vector2i(cols-1, lastLine - 1));
    walls.push_back(sf::Vector2i(cols-1, lastLine - 2));
    walls.push_back(sf::Vector2i(cols-1, lastLine - 3));

    walls.push_back(sf::Vector2i(cols >>2, lastLine - 2));
    walls.push_back(sf::Vector2i(cols >>2, lastLine - 3));
    walls.push_back(sf::Vector2i(cols >>2, lastLine - 4));
    walls.push_back(sf::Vector2i((cols >> 2) + 1, lastLine - 4));
    cacheWalls();
}


/* METHODS */

void Map::cacheWalls()
{
    wallSprites.clear();
    for (sf::Vector2i & wall : walls) {
        sf::RectangleShape rect(sf::Vector2f(16,16));
        rect.setPosition((float)wall.x * C::GRID_SIZE, (float)wall.y * C::GRID_SIZE);
        rect.setFillColor(sf::Color(0x07ff07ff));
        wallSprites.push_back(rect);
    }
}

void Map::addWall(int x, int y)
{
    walls.push_back(sf::Vector2i(x, y));
    cacheWalls();
}

void Map::imgui()
{
    // Map header
    if (ImGui::CollapsingHeader("Map"))
    {
        // Save map in a file
        if (ImGui::Button("SAVE MAP")) {
            FILE* f = fopen("map.txt", "w");
            for (sf::Vector2i & wall : walls) {
                fprintf(f, "%d %d\n", wall.x, wall.y);
            }
            fclose(f);
        }
        // Load map from a file
        if (ImGui::Button("LOAD MAP")) {
            FILE* f = fopen("map.txt", "r");
            walls.clear();
            while (!feof(f)) {
                int x, y;
                fscanf(f, "%d %d\n", &x, &y);
                walls.push_back(sf::Vector2i(x, y));
            }
            fclose(f);
            cacheWalls();
        }
    }
    
}
