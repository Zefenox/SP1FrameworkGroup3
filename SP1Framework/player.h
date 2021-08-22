#pragma once
#include "game.h"
#include "Item.h"
#include "Consumable.h"
#include "HealthPotion.h"
#include "ExtraLife.h"
#include "OddPotion.h"
#include "Cheese.h"

class Player
{
private:
	SHORT health;
	SHORT maxHealth;
	SHORT lives;
	COORD position;
	COORD spawnPoint;
	CHAR direction; // WASD
	WORD charColour;
	bool active;
	Consumable* inventory[5];

public:
	SHORT getHealth();
	void setHealth(SHORT health);
	SHORT getMaxHealth();
	void setMaxHealth(SHORT maxHealth);
	SHORT getLives();
	void setLives(SHORT lives);
	COORD getPosition(); // different ways to set and get the player's pos
	void setPosition(COORD position);
	void setPosition(SHORT X, SHORT Y);
	SHORT getX();
	void setX(SHORT X);
	SHORT getY();
	void setY(SHORT Y);
	Consumable* getInventory(int i);
	void setInventory(int i, Consumable* consumable);
	COORD getSpawnPoint();
	void setSpawnPoint(COORD spawnPoint);
	void setSpawnPoint(SHORT X, SHORT Y);
	CHAR getDirection();
	void setDirection(CHAR direction);
	WORD getCharColour();
	void setCharColour(WORD charColour);
	bool getActive();
	void setActive(bool active);
	void consume(Consumable* consumable);
	void PlayerUpdate();
	Player();
	~Player();
};
