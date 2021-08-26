// coded by: Sze Ting
#pragma once
#include "game.h"
#include <string>

class Item
{
protected:
	std::string name;
	SHORT id;
public:
	Item();
	virtual ~Item();
	std::string getName();
	SHORT getId();
	void useItem();
};

