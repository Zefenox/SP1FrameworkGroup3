#include "Player.h"


Player::Player()
{
    health = 100;
    maxHealth = 100;
    lives = 3;
    position = { 0, 0 };
    spawnPoint = { 0, 0 };
    active = true;
    inventory[0] = new HealthPotion;
    inventory[1] = nullptr;
    inventory[2] = nullptr;
    inventory[3] = nullptr;
    inventory[4] = nullptr;
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

Consumable* Player::getInventory1()
{
    return inventory[0];
}

void Player::setInventory1(Consumable* consumable)
{
    inventory[0] = consumable;
}

Consumable* Player::getInventory2()
{
    return inventory[1];
}

void Player::setInventory2(Consumable* consumable)
{
    inventory[1] = consumable;
}

Consumable* Player::getInventory3()
{
    return inventory[2];
}

void Player::setInventory3(Consumable* consumable)
{
    inventory[2] = consumable;
}

Consumable* Player::getInventory4()
{
    return inventory[3];
}

void Player::setInventory4(Consumable* consumable)
{
    inventory[3] = consumable;
}

Consumable* Player::getInventory5()
{
    return inventory[4];
}

void Player::setInventory5(Consumable* consumable)
{
    inventory[4] = consumable;
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

void Player::consume(Consumable* consumable)
{
    if (consumable->getId() == 1)
        health += 30;
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
