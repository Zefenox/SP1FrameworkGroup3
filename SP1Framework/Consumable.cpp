#include "Consumable.h" // coded by: Sze Ting

Consumable::Consumable()
{
	canBeConsumed = true;
}

Consumable::~Consumable()
{
}

bool Consumable::getCanBeConsumed()
{
	return canBeConsumed;
}

void Consumable::setCanBeConsumed(bool canBeConsumed)
{
	this->canBeConsumed = canBeConsumed;
}
