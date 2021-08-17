#include "checkpoint.h"

void checkpoint::setSpawn()
{
	spawn = getPlayerPosition();
}

void checkpoint::respawn()
{
	setPlayer(spawn);
}

