#pragma once
#include "game.h"
//#include "player.h"


class checkpoint
{
public:

	void setSpawn(); //set the spawnpoint of the player to recent checkpoint
	void respawn();  //respawns player at last checkpoint
	
private:
	COORD spawn;

};

