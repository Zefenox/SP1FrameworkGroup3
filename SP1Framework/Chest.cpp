#include "Chest.h"

Chest::Chest()
{
	position = { 0, 0 };
}

Chest::~Chest()
{
}

COORD Chest::getPosition()
{
	return position;
}

void Chest::setPosition(COORD position)
{
	this->position = position;
}

void Chest::setPosition(SHORT X, SHORT Y)
{
	position.X = X;
	position.Y = Y;
}

SHORT Chest::getX()
{
	return position.X;
}

void Chest::setX(SHORT X)
{
	position.X = X;
}

SHORT Chest::getY()
{
	return position.Y;
}

void Chest::setY(SHORT Y)
{
	position.Y = Y;
}
