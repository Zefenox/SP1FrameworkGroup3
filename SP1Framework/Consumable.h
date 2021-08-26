// coded by: Sze Ting
#pragma once
#include "Item.h"

class Consumable :
    public Item
{
protected:
    bool canBeConsumed;
public:
    Consumable();
    virtual ~Consumable();
    bool getCanBeConsumed();
    void setCanBeConsumed(bool canBeConsumed);
};

