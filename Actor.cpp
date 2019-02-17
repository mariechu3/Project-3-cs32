#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"

///////ACTOR FUNCTION IMPLEMENTATIONS////////////////
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
////////////PENELOPE FUNCTION IMPLEMENTATIONS//////////////////////
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
	if (m_infectionCount == 500)		//penelope turned into a zombie
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
			if(getWorld()->canMove(getX() - 4.0, getY()))	//check if penelope can move to new spot, left
				moveTo(getX() - 4.0, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if(getWorld()->canMove(getX() + 4.0, getY())) // check if penelope can move to new spot, right
				moveTo(getX() + 4.0, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if(getWorld()->canMove(getX(), getY()+4.0)) //check if penelope can move to new spot
				moveTo(getX(), getY()+4.0);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if(getWorld()->canMove(getX(), getY()-4.0))	//check if penelope can move to new spot
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
///////////ZOMBIE FUNCTION IMPLEMENTATIONS///////////////////////
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
////////////////LANDMINE IMPLEMENTATIONS////////////////////
Landmine::Landmine(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_LANDMINE, startX, startY, right, 0, false, myWorld)
{
}
void Landmine::doSomething() {};
////////////////PIT IMPLEMENTATIONS////////////////////
Pit::Pit(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_PIT, startX, startY, right, 0 , false, myWorld)
{
}
void Pit::doSomething() {};
////////////////FLAME IMPLEMENTATIONS////////////////////
Flame::Flame(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_FLAME, startX, startY, startDirection, 0, false, myWorld)
{
}
void Flame::doSomething() {};
////////////////VOMIT IMPLEMENTATIONS////////////////////
Vomit::Vomit(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_VOMIT, startX, startY, startDirection, 0, false, myWorld)
{

}
void Vomit::doSomething() {};
////////////////GOODIES IMPLEMENTATIONS////////////////////
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
////////////////WALL IMPLEMENTATIONS////////////////////
Wall::Wall(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_WALL, startX, startY, right, 0, true, myWorld)
{

}
void Wall::doSomething() {};
////////////////EXIT IMPLEMENTATIONS////////////////////
Exit::Exit(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_EXIT, startX, startY, right, 1, false, myWorld)
{

}
void Exit::doSomething() {};