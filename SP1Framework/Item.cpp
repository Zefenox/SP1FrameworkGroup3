#include "Item.h"

Item::Item()
{
	name = "";
	id = 0;
}

Item::~Item()
{
}

std::string Item::getName()
{
	return name;
}

SHORT Item::getId()
{
	return id;
}

void Item::useItem()
{
}
