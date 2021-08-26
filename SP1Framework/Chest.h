// coded by: Sze Ting
#pragma once
#include "game.h"

class Chest
{
private:
	COORD position;
public:
	Chest();
	~Chest();
	COORD getPosition();
	void setPosition(COORD position);
	void setPosition(SHORT X, SHORT Y);
	SHORT getX();
	void setX(SHORT X);
	SHORT getY();
	void setY(SHORT Y);
};

