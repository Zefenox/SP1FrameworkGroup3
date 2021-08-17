#include "checkpoint.h"

void checkpoint::setSpawn()
{
	// Sets the spawn to Player's current position 
	spawn = getPlayerPosition();
}

void checkpoint::respawn()
{
	// moves the player to the latest checkpoint
	setPlayer(spawn);
}

