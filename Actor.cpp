#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"

Actor::Actor(const int imageID, int startX, int startY, Direction startDirection, int depth, bool block, StudentWorld* myWorld)
	:GraphObject(imageID, startX, startY, startDirection, depth), m_dead(false), m_world(myWorld), m_block(block) {}
bool Actor::isDead()
{
	return m_dead;
}
void Actor::setDead()
{
	m_dead = true;
	m_block = false;
}
StudentWorld* Actor::getWorld()
{
	return m_world;
}
bool Actor::canBlock()
{
	return m_block;
}
Penelope::Penelope(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_PLAYER, startX, startY, right, 0, true, myWorld)
{
	m_landmine = 0; 
	m_flamethrower = 0;
	m_vaccine = 0;
	m_infected = false;
	m_infectionCount = 0;
}
void Penelope::doSomething()
{
	if (isDead())
		return;
	if (m_infected)
		m_infectionCount++;
	if (m_infectionCount == 500)
	{
		setDead();
		getWorld()->decLives();
		//implement code to turn her into zombie;
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	/*implement flamethrower, landmine, vaccine*/
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if(getWorld()->canMove(getX() - 4.0, getY()))
				moveTo(getX() - 4.0, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if(getWorld()->canMove(getX() + 4.0, getY()))
				moveTo(getX() + 4.0, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if(getWorld()->canMove(getX(), getY()+4.0))
				moveTo(getX(), getY()+4.0);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if(getWorld()->canMove(getX(), getY()-4.0))
				moveTo(getX(), getY()-4.0);
			break;
		case KEY_PRESS_SPACE:
			break;
		case KEY_PRESS_TAB:
			break;
		case KEY_PRESS_ENTER:
			break;
		}
	}
}
Zombie::Zombie(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_ZOMBIE, startX, startY, right, 0, true, myWorld)
{

}
void Zombie::doSomething() {};
DumbZombie::DumbZombie(int startX, int startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{

}
void DumbZombie::doSomething() {};
SmartZombie::SmartZombie(int startX, int startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{

}
void SmartZombie::doSomething() {};
Landmine::Landmine(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_LANDMINE, startX, startY, right, 0, false, myWorld)
{
}
void Landmine::doSomething() {};
Pit::Pit(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_PIT, startX, startY, right, 0 , false, myWorld)
{
}
void Pit::doSomething() {};
Flame::Flame(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_FLAME, startX, startY, startDirection, 0, false, myWorld)
{
}
void Flame::doSomething() {};
Vomit::Vomit(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_VOMIT, startX, startY, startDirection, 0, false, myWorld)
{

}
void Vomit::doSomething() {};
Goodies::Goodies(const int imageID, int startX, int startY, StudentWorld* myWorld)
	: Actor(imageID, startX, startY, right, 1, false, myWorld)
{

}
void Goodies::doSomething() {};
VaccineGoodies::VaccineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void VaccineGoodies::doSomething() {};
GasCanGoodies::GasCanGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void GasCanGoodies::doSomething() {};
LandmineGoodies::LandmineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void LandmineGoodies::doSomething() {};
Wall::Wall(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_WALL, startX, startY, right, 0, true, myWorld)
{

}
void Wall::doSomething() {};
Exit::Exit(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_EXIT, startX, startY, right, 1, false, myWorld)
{

}
void Exit::doSomething() {};
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
