#include "Bullet.h"
#include "game.h"
#include "Framework/console.h"
#include "player.h"
#include <iostream>





Bullet::Bullet(int x, int y, char direction)
{
	this->bdirection = direction;

	X = x;
	Y = y;
}

Bullet::~Bullet()
{
	
}

//void Bullet::shoot(char dir)
//{
//	/*if (g_skKeyEvent[K_SPACE].keyReleased)
//	{
//		Bullet* Bullet = new Bullet;
//		Bullet->Bdirection = dir;
//
//	}*/
//	
//	
//}

void Bullet::print()
{
	COORD c;

	c.X = X;
	c.Y = Y;

	if (X >= 0 && X < 300 && Y >= 0 && Y < 65)
	{
		g_Console.writeToBuffer(c, "=", 0x3F);
	}
}

int Bullet::getX()
{
	return X;
}

int Bullet::getY()
{
	return Y;
}

void Bullet::updatebulletpos()
{
	if (bdirection == 'U')
	{
		Y -= 1;
	}
	if (bdirection == 'D')
	{
		Y += 1;
	}
	if (bdirection == 'L')
	{
		X -= 1;
	}
	if (bdirection == 'R')
	{
		X += 1;
	}
}

