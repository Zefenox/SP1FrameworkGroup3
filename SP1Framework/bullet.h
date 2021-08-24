#pragma once
#include "game.h"
#include "Framework/console.h"
class Bullet
{
public:

	
	
	Bullet(int x,int y,char direction);
	~Bullet();
	void print();

	int getX();
	int getY();



	void updatebulletpos();
	

	char bdirection;
	int X,Y;
	

private:
	
	
};


