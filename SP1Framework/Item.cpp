#include "Item.h" // coded by: Sze Ting

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
