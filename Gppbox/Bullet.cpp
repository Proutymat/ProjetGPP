#include "Bullet.hpp"


Bullet::Bullet(int direction, int x, int y, float gameTime) : direction(direction), lastTimeUpdate(gameTime), sprite(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE))
{
    setPositions(x, y);
    sprite.setPosition(xx, yy);
    sprite.setFillColor(sf::Color(sf::Color::Yellow));
    
}

bool Bullet::isWall(std::vector<sf::Vector2i> walls, int cx, int cy)
{
    // Check each wall
    for (sf::Vector2i & wall : walls) {
        if (wall.x == cx && wall.y == cy)
            return true;
    }
    return false;
}

bool Bullet::collideWith(Entity& other)
{
    if (cx == other.cx && cy == other.cy) {
        return true;
    }
    else
    {
        return false;
    }
}

void Bullet::setPositions(float x, float y)
{
    xx = x;	
    yy = y;	
    cx = static_cast<int>(xx/C::GRID_SIZE);	
    cy = static_cast<int>(yy/C::GRID_SIZE);	
    xr = (xx - cx*C::GRID_SIZE) / C::GRID_SIZE;	
    yr = (yy - cy*C::GRID_SIZE) / C::GRID_SIZE;
}

bool Bullet::update(float deltaTime, std::vector<sf::Vector2i> walls, float gameTime)
{
    timePassed += gameTime - lastTimeUpdate;
    lastTimeUpdate = gameTime;
    
    // Destroy bullet if lifetime is over
    if (timePassed >= lifeTime)
        return true;
    
    moveX = 42 * direction;

    // HORIZONTAL MOVEMENT
	
    xr += moveX * deltaTime;

    // Right check
    if (isWall(walls, cx+1,cy) && xr >= 0.0f) {
        xr = 0;
        moveX = 0;
        return true;
    }
    // Left check
    if (isWall(walls, cx-1,cy) && xr <= 0.0f) {
        xr = 0.0f;
        moveX = 0;
        return true;
    }

	
    // UPDATE POSITION
	
    while( xr>1 ) {	xr --;	cx ++;}
    while( xr<0 ) {	xr ++;	cx --;}
    xx = (cx + xr) * C::GRID_SIZE;

    sprite.setPosition(xx, yy);

    return false;
}

void Bullet::draw(sf::RenderWindow& win) const
{
    win.draw(sprite);
}