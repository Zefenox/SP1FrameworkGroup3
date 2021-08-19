#include "Player.h"


Player::Player()
{
    health = 100;
    maxHealth = 100;
    lives = 3;
    position = { 0, 0 };
    spawnPoint = { 0, 0 };
    active = true;
}

Player::~Player()
{
}

SHORT Player::getHealth()
{
    return health;
}

void Player::setHealth(SHORT health)
{
    this->health = health;
}

SHORT Player::getMaxHealth()
{
    return maxHealth;
}

void Player::setMaxHealth(SHORT maxHealth)
{
    this->maxHealth = maxHealth;
}

SHORT Player::getLives()
{
    return lives;
}

void Player::setLives(SHORT lives)
{
    this->lives = lives;
}

COORD Player::getPosition()
{
    return position;
}

void Player::setPosition(COORD position)
{
    this->position = position;
}

void Player::setPosition(SHORT X, SHORT Y)
{
    position.X = X;
    position.Y = Y;
}

SHORT Player::getX()
{
    return position.X;
}

void Player::setX(SHORT X)
{
    position.X = X;
}

SHORT Player::getY()
{
    return position.Y;
}

void Player::setY(SHORT Y)
{
    position.Y = Y;
}

COORD Player::getSpawnPoint()
{
    return spawnPoint;
}

void Player::setSpawnPoint(COORD spawnPoint)
{
    this->spawnPoint = spawnPoint;
}

void Player::setSpawnPoint(SHORT X, SHORT Y)
{
    spawnPoint.X = X;
    spawnPoint.Y = Y;
}

bool Player::getActive()
{
    return active;
}

void Player::setActive(bool active)
{
    this->active = active;
}

void Player::PlayerUpdate()
{
    if (health <= 0 && lives <= 0)
    {
        active = false;
        return;
    }

    if (health <= 0 && active)
    {
        lives--;
        health = maxHealth;
        position = spawnPoint;
    }
}
