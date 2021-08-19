#pragma once
#include "game.h"
#include "Item.h"
#include "Consumable.h"
#include "HealthPotion.h"

class Player
{
private:
	SHORT health;
	SHORT maxHealth;
	SHORT lives;
	COORD position;
	COORD spawnPoint;
	bool active;
	Consumable* inventory[5];

public:
	SHORT getHealth();
	void setHealth(SHORT health);
	SHORT getMaxHealth();
	void setMaxHealth(SHORT maxHealth);
	SHORT getLives();
	void setLives(SHORT lives);
	COORD getPosition();
	void setPosition(COORD position);
	void setPosition(SHORT X, SHORT Y);
	SHORT getX();
	void setX(SHORT X);
	SHORT getY();
	void setY(SHORT Y);
	Consumable* getInventory1();
	void setInventory1(Consumable* consumable);
	Consumable* getInventory2();
	void setInventory2(Consumable* consumable);
	Consumable* getInventory3();
	void setInventory3(Consumable* consumable);
	Consumable* getInventory4();
	void setInventory4(Consumable* consumable);
	Consumable* getInventory5();
	void setInventory5(Consumable* consumable);
	COORD getSpawnPoint();
	void setSpawnPoint(COORD spawnPoint);
	void setSpawnPoint(SHORT X, SHORT Y);
	bool getActive();
	void setActive(bool active);
	void consume(Consumable* consumable);
	void PlayerUpdate();
	Player();
	~Player();
};
