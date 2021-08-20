#include "Player.h"

Player::Player()
{
    health = 100;
    maxHealth = 100;
    lives = 3;
    position = { 0, 0 };
    spawnPoint = { 0, 0 };
    direction = 'W';
    charColour = 0x84;
    active = true;
    inventory[0] = new HealthPotion;
    inventory[1] = new ExtraLife;
    inventory[2] = new OddPotion;
    inventory[3] = new Cheese;
    inventory[4] = new Cheese;
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

Consumable* Player::getInventory(int i)
{
    return inventory[i];
}

void Player::setInventory(int i, Consumable* consumable)
{
    inventory[i] = consumable;
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

CHAR Player::getDirection()
{
    return direction;
}

void Player::setDirection(CHAR direction)
{
    this->direction = direction;
}

WORD Player::getCharColour()
{
    return charColour;
}

void Player::setCharColour(WORD charColour)
{
    this->charColour = charColour;
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
    if (consumable->getId() == 1) // health potion
        health += 30;
    if (consumable->getId() == 2) // extra life
        lives += 1;
    if (consumable->getId() == 3) // odd potion (random effect)
    {
        int randNum = (rand() % 4) + 1;
        switch (randNum)
        {
            case 1:
            {
                health += 20;
                break;
            }
            case 2:
            {
                health -= 20;
                break;
            }
            case 3:
            {
                lives -= 1;
                break;
            }
            case 4:
            {
                maxHealth += 30;
                health += 30;
                break;
            }
        }
 
    }

    if (consumable->getId() == 4) // cheese
    {
        health = maxHealth;
    }
}

void Player::PlayerUpdate()
{
    if (health > maxHealth) // capping health
        health = maxHealth;

    if (health < 0) // removing negative health
        health = 0;

    if (lives <= 0)
    {
        active = false;
        return;
    }

    if (health <= 0)
    {
        lives--;
        if (lives > 0)
        {
            health = maxHealth;
            position = spawnPoint;
        }
    }
}
