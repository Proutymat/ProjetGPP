#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"

Entity::Entity(){};

Entity::Entity(Game* game, int cx, int cy, bool isEnemy)
	: sprite(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE)),
	  cx(cx), cy(cy), xr(0), yr(0),
	  xx(0), yy(0), moveX(0), moveY(0),
	  game(game)
{
	sprite.setPosition(xx, yy);
	if (isEnemy) sprite.setFillColor(sf::Color(sf::Color::Red));
	else sprite.setFillColor(sf::Color(sf::Color::White));
}

void Entity::setPositions(float x, float y)
{
	xx = x;	
	yy = y;	
	cx = static_cast<int>(xx/C::GRID_SIZE);	
	cy = static_cast<int>(yy/C::GRID_SIZE);	
	xr = (xx - cx*C::GRID_SIZE) / C::GRID_SIZE;	
	yr = (yy - cy*C::GRID_SIZE) / C::GRID_SIZE;
}

void Entity::applyMovement(double detlaTime)
{
	// VERTICAL MOVEMENT
	yr += moveY * detlaTime;

	// Roof check, only reset direction
	if (game->isWall(cx, cy-1) && yr < 0.0f) {
		moveY = 0;
	}
	// Ground check, reset both direction and position
	if (game->isWall(cx, cy+1) && yr >= 0.0f) {
		moveY = 0;
		yr = 0.0f;
		onGround = true;
	} else {
		onGround = false;
	}
	
	moveY += game->gravity * detlaTime; // Apply gravity

	while(yr > 1) {cy++; yr--;}
	while(yr < 0) {cy--; yr++;}


	// HORIZONTAL MOVEMENT
	
	xr += moveX * detlaTime;
	moveX *= 0.96f;

	// Right check
	if (game->isWall(cx+1,cy) && xr >= 0.0f) {
		xr = 0;
		moveX = 0;
		direction = -1; // IA Mustache
	}
	// Left check
	if (game->isWall(cx-1,cy) && xr <= 0.0f) {
		xr = 0.0f;
		moveX = 0;
		direction = 1; // IA Mustache
	}

	
	// UPDATE POSITION
	
	while( xr>1 ) {	xr --;	cx ++;}
	while( xr<0 ) {	xr ++;	cx --;}
	xx = (cx + xr) * C::GRID_SIZE;
	yy = (cy + yr) * C::GRID_SIZE;
	
	sprite.setPosition(xx, yy);
}

bool Entity::collideWith(Entity& other)
{
    if (cx == other.cx && cy == other.cy) {
        return true;
    }
    else
    {
	    return false;
    }
}

void Entity::draw(sf::RenderWindow& win) const
{
	win.draw(sprite);
}