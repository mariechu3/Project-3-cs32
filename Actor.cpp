#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <iostream>
using namespace std;

///////ACTOR FUNCTION IMPLEMENTATIONS////////////////
Actor::Actor(const int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* myWorld)
	:GraphObject(imageID, startX, startY, startDirection, depth), m_dead(false), m_world(myWorld) {}
bool Actor::isDead()
{
	return m_dead;
}
void Actor::setDead()			//sets an actor state to dead and makes sure they can't block another actor
{
	m_dead = true;
}
StudentWorld* Actor::getWorld()
{
	return m_world;
}
bool Actor::canBlock()
{
	return false;
}
bool Actor::isPerson()
{
	return false;
}
bool Actor::isZombie()
{
	return false;
}
bool Actor::blockFlame()
{
	if (affectedByFlame())
		return false;
	return true;
}
bool Actor::affectedByFlame()
{
	return true;
}
void Actor::death()
{
	setDead();
}
Person::Person(const int imageID, int startX, int startY, StudentWorld* myWorld)
	:Actor(imageID, startX, startY, right, 0, myWorld)
{
	m_infected = false;
	m_infectionCount = 0;
}
bool Person::canBlock()
{
	return true;
}
bool Person::isPerson()
{
	return true;
}
void Person::setInfected(bool yes)
{
	m_infected = yes;
}
void Person::addInfection()
{
	m_infectionCount ++;
}
void Person::setInfectionBack()
{
	m_infectionCount = 0;
}
int Person::infectionCount()
{
	return m_infectionCount;
}
bool Person::infected()
{
	return m_infected;
}
////////////PENELOPE FUNCTION IMPLEMENTATIONS//////////////////////
Penelope::Penelope(int startX, int startY, StudentWorld* myWorld)
	:Person(IID_PLAYER, startX, startY, myWorld)
{
	m_landmine = 0; 
	m_flamethrower = 0;
	m_vaccine = 0;
	m_completed = false;
}
int Penelope::numLandmine()
{
	return m_landmine;
}
void Penelope::addNumLand(int add)
{
	m_landmine += add;
}
int Penelope::numFlames()
{
	return m_flamethrower;
}
void Penelope::addNumFlames(int add)
{
	m_flamethrower += add;
}
int Penelope::numVaccine()
{
	return m_vaccine;
}
void Penelope::addNumVaccine(int add)
{
	m_vaccine += add;
}
void Penelope::setCompletion()
{
	m_completed = true;
}
bool Penelope::completion()
{
	return m_completed;
}
void Penelope::doSomething()
{
	if (isDead())
		return;
	if (infected())
	{
		addInfection();
	}
	if (infectionCount() == 500)		//penelope turned into a zombie
	{
		setDead();
		getWorld()->decLives();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if(getWorld()->canMove(this, getX() - 4.0, getY()))	//check if penelope can move to new spot, left
				moveTo(getX() - 4.0, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if(getWorld()->canMove(this, getX() + 4.0, getY())) // check if penelope can move to new spot, right
				moveTo(getX() + 4.0, getY());
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if(getWorld()->canMove(this, getX(), getY()+4.0)) //check if penelope can move to new spot
				moveTo(getX(), getY()+4.0);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if(getWorld()->canMove(this, getX(), getY()-4.0))	//check if penelope can move to new spot
				moveTo(getX(), getY()-4.0);
			break;
		case KEY_PRESS_SPACE:
			if (m_flamethrower > 0)
			{
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				Direction dir = getDirection();
				switch (dir)
				{
				case up:
					for (double i = 1; i <= 3; i++)
					{
						if(getWorld()->blockFlame(getX(), getY() + (i*SPRITE_HEIGHT)))
							break;
						getWorld()->addActor('f', getX(), getY() + (i*SPRITE_HEIGHT), up, getWorld());
					}
					break;
				case down:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX(), getY() - (i*SPRITE_HEIGHT)))
							break;
						getWorld()->addActor('f', getX(), getY() - (i*SPRITE_HEIGHT), down, getWorld());
					}
					break;
				case left:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX() - (i*SPRITE_WIDTH), getY()))
							break;
						getWorld()->addActor('f', getX() - (i*SPRITE_WIDTH), getY(), left, getWorld());
					}
					break;
				case right:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX() + (i*SPRITE_WIDTH), getY()))
							break;
						getWorld()->addActor('f', getX() + (i*SPRITE_WIDTH), getY(), right, getWorld());
					}
					break;
				}
				m_flamethrower--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_landmine > 0)
			{
				getWorld()->addActor('l', getX(), getY(), right, getWorld());
				m_landmine--;
			}
			break;
		case KEY_PRESS_ENTER:
			if (m_vaccine > 0)
			{
				m_vaccine--;
				setInfected(false);
				setInfectionBack();
			}
			break;
		}
	}
}
////////////CITIZEN IMPLEMENTATIONS///////////////////
Citizen::Citizen(int startX, int startY, StudentWorld* myWorld)
	: Person(IID_CITIZEN, startX, startY, myWorld)
{
	m_paralysis = 0;
	firstInfection = true;
}
void Citizen::death()
{
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	getWorld()->increaseScore(-1000);
}
void Citizen::setInfected(bool yes)
{
	Person::setInfected(true);
	if (firstInfection)
	{
		getWorld()->playSound(SOUND_CITIZEN_INFECTED);
		firstInfection = false;
	}
}
void Citizen::doSomething()
{
	if (isDead())
		return;
	if (infected())
	{
		addInfection();
	}
	if (infectionCount() == 500)
	{
		setDead();
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
		if (randInt(1, 10) == 7)
			getWorld()->addActor('d', getX(), getY(), getDirection(), getWorld());
		else
			getWorld()->addActor('s', getX(), getY(), getDirection(), getWorld());
	}
	m_paralysis++;
	if (m_paralysis % 2 == 1)
		return;
	double dist_p = getWorld()->distanceFromPene(getX(), getY());
	double dist_z = getWorld()->distanceFromZombie(getX(), getY());
	if (dist_p < dist_z && dist_p <= 80)
	{
		char row = getWorld()->rowMoveToP(this);
		char col = getWorld()->colMoveToP(this);
		char otherRow = ' ';
		char otherCol = ' ';
		switch (row)
		{
		case 'r':
			if (getWorld()->canMove(this, getX() + 2, getY()))
			{
				setDirection(right);
				moveTo(getX() + 2, getY());
				return;
			}
			break;
		case'l':
			if (getWorld()->canMove(this, getX() - 2, getY()))
			{
				setDirection(left);
				moveTo(getX() - 2, getY());
				return;
			}
			break;
		case'd':
			otherRow = 'd';
			break;
		case 'a':
			otherRow = 'a';
			break;
		}
		switch (col)
		{
		case 'u':
			if (getWorld()->canMove(this, getX(), getY() + 2))
			{
				setDirection(up);
				moveTo(getX(), getY() + 2);
				return;
			}
			break;
		case'b':
			if (getWorld()->canMove(this, getX(), getY() - 2))
			{
				setDirection(down);
				moveTo(getX(), getY() - 2);
				return;
			}
			break;
		case 'w':
			otherCol = 'w';
			break;
		case 's':
			otherCol = 's';
			break;
		}
		char newDir = ' ';
		bool success = false;
		if (randInt(1, 2) == 1)
			newDir = otherRow;
		else
			newDir = otherCol;
		switch (newDir)
		{
		case 'd':
			if (getWorld()->canMove(this, getX() + 2, getY()))
			{
				setDirection(right);
				moveTo(getX() + 2, getY());
				success = true;
				return;
			}
			break;
		case'a':
			if (getWorld()->canMove(this, getX() - 2, getY()))
			{
				setDirection(left);
				moveTo(getX() - 2, getY());
				success = true;
				return;
			}
			break;
		case 'w':
			if (getWorld()->canMove(this, getX(), getY() + 2))
			{
				setDirection(up);
				moveTo(getX(), getY() + 2);
				success = true;
				return;
			}
			break;
		case's':
			if (getWorld()->canMove(this, getX(), getY() - 2))
			{
				setDirection(down);
				moveTo(getX(), getY() - 2);
				success = true;
				return;
			}
			break;
		}
		if (!success)
		{
			if (newDir == otherCol)
				newDir = otherRow;
			else
				newDir = otherCol;
			switch (newDir)
			{
			case 'd':
				if (getWorld()->canMove(this, getX() + 2, getY()))
				{
					setDirection(right);
					moveTo(getX() + 2, getY());
					success = true;
					return;
				}
				break;
			case'a':
				if (getWorld()->canMove(this, getX() - 2, getY()))
				{
					setDirection(left);
					moveTo(getX() - 2, getY());
					success = true;
					return;
				}
				break;
			case 'w':
				if (getWorld()->canMove(this, getX(), getY() + 2))
				{
					setDirection(up);
					moveTo(getX(), getY() + 2);
					success = true;
					return;
				}
				break;
			case's':
				if (getWorld()->canMove(this, getX(), getY() - 2))
				{
					setDirection(down);
					moveTo(getX(), getY() - 2);
					success = true;
					return;
				}
				break;
			}
		}
	}
	else if (dist_z <= 80)
	{
		double moveDistance = dist_z;
		char move = ' ';
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX() + 2, getY()) > moveDistance) //checks right
		{
			moveDistance = getWorld()->distanceFromZombie(getX() + 2, getY());
			move = 'r';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX() - 2, getY()) > moveDistance) //checks left
		{
			moveDistance = getWorld()->distanceFromZombie(getX() - 2, getY());
			move = 'l';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX(), getY() + 2) > moveDistance) //checks up
		{
			moveDistance = getWorld()->distanceFromZombie(getX(), getY() + 2);
			move = 'u';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX(), getY() - 2) > moveDistance) //checks down
		{
			moveDistance = getWorld()->distanceFromZombie(getX(), getY() - 2);
			move = 'd';
		}
		if (moveDistance == dist_z)
			return;
		else
		{
			switch (move)
			{
			case 'r':
				if (getWorld()->canMove(this, getX() + 2, getY()))
				{
					setDirection(right);
					moveTo(getX() + 2, getY());
					return;
				}
				break;
			case'l':
				if (getWorld()->canMove(this, getX() - 2, getY()))
				{
					setDirection(left);
					moveTo(getX() - 2, getY());
					return;
				}
				break;
			case 'u':
				if (getWorld()->canMove(this, getX(), getY() + 2))
				{
					setDirection(up);
					moveTo(getX(), getY() + 2);
					return;
				}
				break;
			case'd':
				if (getWorld()->canMove(this, getX(), getY() - 2))
				{
					setDirection(down);
					moveTo(getX(), getY() - 2);
					return;
				}
				break;
			}
		}
	}
}
///////////ZOMBIE FUNCTION IMPLEMENTATIONS///////////////////////
Zombie::Zombie(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_ZOMBIE, startX, startY, right, 0, myWorld)
{
	m_paralysis = 0;
	m_movement = 0;
}
bool Zombie::canBlock()
{
	return true;
}
void Zombie::frontCoord(double &xPos, double &yPos, Direction dir)
{
	switch (dir)
	{
	case right:
		xPos += SPRITE_WIDTH;
		break;
	case left:
		xPos -= SPRITE_WIDTH;
		break;
	case up:
		yPos += SPRITE_HEIGHT;
		break;
	case down:
		yPos -= SPRITE_HEIGHT;
		break;
	}
}
bool Zombie::isZombie()
{
	return true;
}
void Zombie::doSomething() 
{
	if (isDead())
		return;
	m_paralysis++;
	if (m_paralysis % 2 == 0)
	{
		double xPos = getX();
		double yPos = getY();
		frontCoord(xPos, yPos, getDirection());
		if (getWorld()->ifPersonInFront(xPos, yPos) && randInt(1, 3) == 3)
		{
			getWorld()->addActor('v', xPos, yPos, getDirection(), getWorld());
			getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			return;
		}
		if (m_movement == 0)
		{
			m_movement = randInt(3, 10);
			if (!differentMovements())
			{
				int temp = randInt(1, 4);
				switch (temp)
				{
				case 1:
					setDirection(up);
					break;
				case 2:
					setDirection(down);
					break;
				case 3:
					setDirection(right);
					break;
				case 4:
					setDirection(left);
					break;
				}
			}
			else
			{
				differentMovements();
			}
		}
		else
		{
			int temp = getDirection();
			switch (temp)
			{
			case right:
				if (getWorld()->canMove(this, getX() + 1.0, getY()))
					moveTo(getX() + 1, getY());
				else
				{
					m_movement = 0;
					return;
				}
				break;
			case left:
				if (getWorld()->canMove(this, getX() - 1.0, getY()))
					moveTo(getX() - 1, getY());
				else
				{
					m_movement = 0;
					return;
				}
				break;
			case up:
				if (getWorld()->canMove(this, getX(), getY() + 1.0))
					moveTo(getX(), getY() + 1);
				else
				{
					m_movement = 0;
					return;
				}
				break;
			case down:
				if (getWorld()->canMove(this, getX(), getY() - 1.0))
					moveTo(getX(), getY() - 1);
				else
				{
					m_movement = 0;
					return;
				}
				break;
			}
			m_movement--;
		}
	}

};
DumbZombie::DumbZombie(int startX, int startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{

}
void DumbZombie::death()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	if (randInt(1, 10) == 1)
	{
		int dir = randInt(1, 4);
		switch (dir)
		{
		case 1:
			if (getWorld()->canFlingVaccine(this, getX() + SPRITE_WIDTH, getY()))
				getWorld()->addActor('m', getX() + SPRITE_WIDTH, getY(), right, getWorld());
			break;
		case 2:
			if (getWorld()->canFlingVaccine(this, getX() - SPRITE_WIDTH, getY()))
				getWorld()->addActor('m', getX() - SPRITE_WIDTH, getY(), right, getWorld());
			break;
		case 3:
			if (getWorld()->canFlingVaccine(this, getX(), getY() + SPRITE_HEIGHT))
				getWorld()->addActor('m', getX(), getY() + SPRITE_HEIGHT, right, getWorld());
			break;
		case 4:
			if (getWorld()->canFlingVaccine(this, getX(), getY() - SPRITE_HEIGHT))
				getWorld()->addActor('m', getX(), getY() - SPRITE_HEIGHT, right, getWorld());
			break;
		}
	}
	getWorld()->increaseScore(1000);
}
bool DumbZombie::differentMovements()
{
	return false;
}
SmartZombie::SmartZombie(int startX, int startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{

}
void SmartZombie::death()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(2000);
}
bool SmartZombie::differentMovements()
{
	double xCoor = 0;
	double yCoor = 0;
	int p_dist = getWorld()->distanceFromPene(getX(), getY());
	int c_dist = getWorld()->distanceFromPerson(getX(), getY(),xCoor,yCoor);
	if (c_dist < p_dist)
		p_dist = c_dist;
	if (p_dist > 80)
		return false;
	if (p_dist != c_dist)
	{
		char row = getWorld()->rowMoveToP(this);
		char col = getWorld()->colMoveToP(this);
		char otherRow = ' ';
		char otherCol = ' ';
		switch (row)
		{
		case 'r':
			setDirection(right);
			return true;
			break;
		case'l':
			setDirection(left);
			return true;
			break;
		case'd':
			otherRow = 'd';
			break;
		case 'a':
			otherRow = 'a';
			break;
		}
		switch (col)
		{
		case 'u':
			setDirection(up);
			return true;
			break;
		case'b':
			setDirection(down);
			return true;
			break;
		case 'w':
			otherCol = 'w';
			break;
		case 's':
			otherCol = 's';
			break;
		}
		char newDir = ' ';
		if (randInt(1, 2) == 1)
			newDir = otherRow;
		else
			newDir = otherCol;
		switch (newDir)
		{
		case 'd':
			setDirection(right);
			return true;
			break;
		case'a':
			setDirection(left);
			return true;
			break;
		case 'w':
			setDirection(up);
			return true;
			break;
		case's':
			setDirection(down);
			return true;
			break;
		}
	}
	else
	{
		if (xCoor == getX())
		{
			if (yCoor >= getY())
			{
				setDirection(up);
				return true;
			}
			else
			{
				setDirection(down);
				return true;
			}
		}
		if (yCoor == getY())
		{
			if (xCoor > getX())
			{
				setDirection(right);
				return true;
			}
			else
			{
				setDirection(left);
				return true;
			}			
		}
		int choose = randInt(1, 2);
		if (choose == 1) // in x
		{
			if (xCoor > getX())
			{
				setDirection(right);
				return true;
			}
			else
			{
				setDirection(left);
				return true;
			}
		}
		else //in y
		{
			if (yCoor > getY())
			{
				setDirection(up);
				return true;
			}
			else
			{
				setDirection(down);
				return true;
			}

		}
	}
}
////////////////LANDMINE IMPLEMENTATIONS////////////////////
Landmine::Landmine(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_LANDMINE, startX, startY, right, 1, myWorld)
{
	active = false;
	safety = 30;
	setOff = false;
}
void Landmine::doSomething() 
{
	if (isDead())
		return;
	if (safety > 0)
	{
		safety--;
		if (safety == 0)
		{
			active = true;
			return;
		}
	}
	if (active && getWorld()->stepOnLandmine(getX(), getY()))
		//setOff = true;
	//if (active && setOff)
	//{
		setOffLandmine();
//	}
};
void Landmine::death()
{
	setDead();
	setOffLandmine();
}
void Landmine::setOffLandmine()
{
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	for(double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY() + SPRITE_HEIGHT, up, getWorld());	//row 1 of fire
	for (double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY(), up, getWorld());   //row 2 of fire
	for (double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY() -SPRITE_HEIGHT, up, getWorld());	//row 3 of fire
	getWorld()->addActor('p', getX(), getY(), right, getWorld());
	setDead();
	active = false;
}
////////////////PIT IMPLEMENTATIONS////////////////////
Pit::Pit(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_PIT, startX, startY, right, 0 , myWorld)
{
}
bool Pit::affectedByFlame()
{
	return false;
}
bool Pit::blockFlame()
{
	return false;
}
void Pit::doSomething() 
{
	getWorld()->dieByPitOrFlame(getX(), getY());
};
////////////////FLAME IMPLEMENTATIONS////////////////////
Flame::Flame(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_FLAME, startX, startY, startDirection, 0, myWorld)
{
	m_lives = 2;
}
bool Flame::affectedByFlame()
{
	return false;
}
void Flame::doSomething()
{
	if (isDead())
		return;
	m_lives--;
	if (m_lives == 0)
	{
		setDead();
		return;
	}
	getWorld()->dieByPitOrFlame(getX(), getY());
};
////////////////VOMIT IMPLEMENTATIONS////////////////////
Vomit::Vomit(int startX, int startY, Direction startDirection, StudentWorld* myWorld)
	: Actor(IID_VOMIT, startX, startY, startDirection, 0, myWorld)
{
	m_lives = 2;
}
bool Vomit::affectedByFlame()
{
	return false;
}
void Vomit::doSomething() 
{
	if (isDead())
		return;
	m_lives--;
	if (m_lives == 0)
	{
		setDead();
		return;
	}
	getWorld()->infect(getX(), getY());

};
////////////////GOODIES IMPLEMENTATIONS////////////////////
Goodies::Goodies(const int imageID, int startX, int startY, StudentWorld* myWorld)
	: Actor(imageID, startX, startY, right, 1, myWorld)
{

}
void Goodies::doSomething() 
{
	if (isDead())
		return;
	if (getWorld()->overlapsPene(getX(), getY()))
	{
		getWorld()->increaseScore(50);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		addCount();
	}
};
VaccineGoodies::VaccineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void VaccineGoodies::addCount() 
{
	getWorld()->increaseCount(1, 'v');	
};
GasCanGoodies::GasCanGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void GasCanGoodies::addCount() 
{
	getWorld()->increaseCount(5, 'f');
};
LandmineGoodies::LandmineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{
}
void LandmineGoodies::addCount() 
{
	getWorld()->increaseCount(2, 'l');
}
////////////////WALL IMPLEMENTATIONS////////////////////
Wall::Wall(int startX, int startY, StudentWorld* myWorld)
	: Actor(IID_WALL, startX, startY, right, 0, myWorld)
{

}
bool Wall::canBlock()
{
	return true;
}
bool Wall::affectedByFlame()
{
	return false;
}
void Wall::doSomething() {};
////////////////EXIT IMPLEMENTATIONS////////////////////
Exit::Exit(int startX, int startY, StudentWorld* myWorld)
	:Actor(IID_EXIT, startX, startY, right, 1, myWorld)
{

}
bool Exit::affectedByFlame()
{
	return false;
}
void Exit::doSomething() 
{		
	if (getWorld()->citizenCount() == 0)
	{
		getWorld()->canLeave(getX(), getY());
	}
	if (getWorld()->left(getX(), getY()))
	{
		getWorld()->increaseScore(500);
		getWorld()->playSound(SOUND_CITIZEN_SAVED);
	}	
};