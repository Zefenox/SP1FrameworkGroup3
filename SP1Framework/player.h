#pragma once
#include "game.h"

class Player
{
private:
	SHORT health;
	SHORT maxHealth;
	SHORT lives;
	COORD position;
	COORD spawnPoint;
	bool active;

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
	COORD getSpawnPoint();
	void setSpawnPoint(COORD spawnPoint);
	void setSpawnPoint(SHORT X, SHORT Y);
	bool getActive();
	void setActive(bool active);
	void PlayerUpdate();
	Player();
	~Player();
};
