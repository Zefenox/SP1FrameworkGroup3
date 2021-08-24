#pragma once
#include "game.h"
#include "Framework/console.h"
class Bullet
{
public:

	
	
	Bullet(int x,int y,char direction);
	~Bullet();
	void print();



	void updatebulletpos();
	

	char bdirection;
	int X,Y;
	

private:
	
	
};


