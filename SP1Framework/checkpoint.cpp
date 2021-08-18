#include "checkpoint.h"





//void checkpoint::setSpawn()
//{
//	 //Sets the spawn to Player's current position 
//	spawn = player::getPosition();
//}

void checkpoint::respawn()
{
	// moves the player to the latest checkpoint
	setPlayer(spawn);
}

