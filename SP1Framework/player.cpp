#include "player.h"

player::player()
{
    health = 100;
    lives = 3;
    active = true;
    position.X = 0;
    position.Y = 0;
}

player::~player()
{
}

SHORT player::getHealth()
{
    return health;
}

void player::setHealth(SHORT health)
{
    this->health = health;
}

SHORT player::getLives()
{
    return lives;
}

void player::setLives(SHORT lives)
{
    this->lives = lives;
}

COORD player::getPosition()
{
    return position;
}

void player::setPosition(COORD position)
{
    this->position = position;
}
