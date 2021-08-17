#pragma once
#include "game.h"

class player
{
private:
	SHORT health;
	SHORT lives;
	COORD position;
	bool active;

public:
	SHORT getHealth();
	void setHealth(SHORT health);
	SHORT getLives();
	void setLives(SHORT lives);
	COORD getPosition();
	void setPosition(COORD position);
	player();
	~player();

};

