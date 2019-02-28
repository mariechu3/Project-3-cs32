#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <iostream>
using namespace std;

///////ACTOR FUNCTION IMPLEMENTATIONS////////////////
Actor::Actor(const int imageID, double startX, double startY, Direction startDirection, int depth, StudentWorld* myWorld)
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
	return false;	//for most actors they can't block other actors
}
bool Actor::isPerson()
{
	return false;	//most actors are not people
}
bool Actor::isZombie()
{
	return false;	//most actors are not zombies
}
bool Actor::blockFlame()
{
	if (affectedByFlame())
		return false;		//most actors affected by Flame cannot block flames
	return true;	
}
bool Actor::affectedByFlame()
{
	return true;			//most actors are affected by Flames
}
void Actor::death()
{
	setDead();		//most actors just die when they are killed
}
//////////////////////////////////PERSON IMPLEMENTATIONS//////////////////////////////////////////
Person::Person(const int imageID, double startX, double startY, StudentWorld* myWorld)
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
Penelope::Penelope(double startX, double startY, StudentWorld* myWorld)
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
		addInfection();	//if she is infected, increase her infection count
	}
	if (infectionCount() == 500)		//penelope turned into a zombie/died
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
			if(getWorld()->canMove(this, getX(), getY()+4.0)) //check if penelope can move to new spot, up
				moveTo(getX(), getY()+4.0);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if(getWorld()->canMove(this, getX(), getY()-4.0))	//check if penelope can move to new spot, down
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
						if(getWorld()->blockFlame(getX(), getY() + (i*SPRITE_HEIGHT)))	//checks if flame is blocked
							break;
						getWorld()->addActor('f', getX(), getY() + (i*SPRITE_HEIGHT), up, getWorld());	//creates 3 flames in up dir if not blocked
					}
					break;
				case down:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX(), getY() - (i*SPRITE_HEIGHT)))	//checks if flame is blocked
							break;
						getWorld()->addActor('f', getX(), getY() - (i*SPRITE_HEIGHT), down, getWorld()); //creates 3 flames in down dir if not blocked
					}
					break;
				case left:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX() - (i*SPRITE_WIDTH), getY()))
							break;
						getWorld()->addActor('f', getX() - (i*SPRITE_WIDTH), getY(), left, getWorld());		//creates 3 flames in left dir if not blocked
					}
					break;
				case right:
					for (double i = 1; i <= 3; i++)
					{
						if (getWorld()->blockFlame(getX() + (i*SPRITE_WIDTH), getY()))
							break;
						getWorld()->addActor('f', getX() + (i*SPRITE_WIDTH), getY(), right, getWorld());	//creates 3 flames in the right dir if not blocked
					}
					break;
				}
				m_flamethrower--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_landmine > 0)	//sets down a landmine
			{
				getWorld()->addActor('l', getX(), getY(), right, getWorld());
				m_landmine--;
			}
			break;
		case KEY_PRESS_ENTER:
			if (m_vaccine > 0)		//uses her vaccine
			{
				m_vaccine--;
				setInfected(false);
				setInfectionBack();
			}
			break;
		}
	}
}
////////////CITIZEN IMPLEMENTATIONS///////////////////////////////////
Citizen::Citizen(double startX, double startY, StudentWorld* myWorld)
	: Person(IID_CITIZEN, startX, startY, myWorld)
{
	m_paralysis = 0;
	firstInfection = true;
}
void Citizen::death()	//what a citizen does if it dies by pit/flames
{
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	getWorld()->increaseScore(-1000);
}
void Citizen::setInfected(bool yes)
{
	Person::setInfected(true);
	if (firstInfection)		// if it is the first time a citizen was infection, play the sound
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
			getWorld()->addActor('d', getX(), getY(), getDirection(), getWorld());		//70% chance dumb zombie created
		else
			getWorld()->addActor('s', getX(), getY(), getDirection(), getWorld());		//30% chance smart zombie created
	}
	m_paralysis++;
	if (m_paralysis % 2 == 1)		//citizen only does something every other tick
		return;
	double dist_p = getWorld()->distanceFromPene(getX(), getY());
	double dist_z = getWorld()->distanceFromZombie(getX(), getY());
	if (dist_p < dist_z && dist_p <= 80)		//if the distance to penelope is closer than to a zombie
	{
		char row = getWorld()->rowMoveToP(this);		//find which dir citizen should move along the rows
		char col = getWorld()->colMoveToP(this);		//find which dir citizen should move along the col
		char otherRow = ' ';
		char otherCol = ' ';
		switch (row)
		{
		case 'r':
			if (getWorld()->canMove(this, getX() + 2, getY()))	//if the citizen is on the same row as penelope and can move
			{
				setDirection(right);	
				moveTo(getX() + 2, getY()); //move closer to penelope (right)
				return;
			}
			break;
		case'l':
			if (getWorld()->canMove(this, getX() - 2, getY())) //if citizen is on the same row as penelope and can move
			{
				setDirection(left);
				moveTo(getX() - 2, getY()); //move closer to penelope (left)
				return;
			}
			break;
		case'd':
			otherRow = 'd';		//citizen is not on same row, penelope is to the right
			break;
		case 'a':
			otherRow = 'a'; //citizen is not on the same row, penelope is to the left
			break;
		}
		switch (col)
		{
		case 'u':
			if (getWorld()->canMove(this, getX(), getY() + 2))	//if citizen is on the same col as penelope and can move
			{
				setDirection(up);
				moveTo(getX(), getY() + 2);  //move closer to penelope (up)
				return;
			}
			break;
		case'b':
			if (getWorld()->canMove(this, getX(), getY() - 2)) //if citizen is on the same col as penelope and can move
			{
				setDirection(down);
				moveTo(getX(), getY() - 2);  //move closer to penelope (down)
				return;
			}
			break;
		case 'w':
			otherCol = 'w';	//citizen not on same col, penelope is above
			break;
		case 's':
			otherCol = 's'; //citizen not on same col, penelope is below
			break;
		}
		//if it reached here that means citizen and penelope are not on the same row/col
		char newDir = ' ';
		bool success = false;
		if (randInt(1, 2) == 1)	//choose randomly between vertical or horizontal dir that will get citizen closer to penelope
			newDir = otherRow;
		else
			newDir = otherCol;
		switch (newDir)
		{
		case 'd':
			if (getWorld()->canMove(this, getX() + 2, getY()))	//if citizen can move
			{
				setDirection(right);
				moveTo(getX() + 2, getY());	//move closer to penelope (right)
				success = true;
				return;
			}
			break;
		case'a':
			if (getWorld()->canMove(this, getX() - 2, getY()))	//if citizen can move
			{
				setDirection(left);
				moveTo(getX() - 2, getY()); //move closer to peneleop (left)
				success = true;
				return;
			}
			break;
		case 'w':
			if (getWorld()->canMove(this, getX(), getY() + 2))	//if citizen can move
			{
				setDirection(up);
				moveTo(getX(), getY() + 2);	//move closer to penelope (up)
				success = true;
				return;
			}
			break;
		case's':
			if (getWorld()->canMove(this, getX(), getY() - 2)) //if citizen can move
			{
				setDirection(down);
				moveTo(getX(), getY() - 2); //move closer to penelope (down)
				success = true;
				return;
			}
			break;
		}
		if (!success)	//if the citizen could not move in the randomly selected horizontal/vertical dir that would get it closer to penelope
		{
			if (newDir == otherCol)
				newDir = otherRow;
			else
				newDir = otherCol;
			switch (newDir)	//try to see if citizen can move in the other direction that will get it closer to penelope
			{
			case 'd':
				if (getWorld()->canMove(this, getX() + 2, getY()))
				{
					setDirection(right);
					moveTo(getX() + 2, getY());
					return;
				}
				break;
			case'a':
				if (getWorld()->canMove(this, getX() - 2, getY()))
				{
					setDirection(left);
					moveTo(getX() - 2, getY());
					return;
				}
				break;
			case 'w':
				if (getWorld()->canMove(this, getX(), getY() + 2))
				{
					setDirection(up);
					moveTo(getX(), getY() + 2);
					return;
				}
				break;
			case's':
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
	else if (dist_z <= 80) // if a zombie is closer than penelope and within 80 pixel distance away
	{
		double moveDistance = dist_z;
		char move = ' ';
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX() + 2, getY()) > moveDistance) //checks right
		{
			moveDistance = getWorld()->distanceFromZombie(getX() + 2, getY());	//sets to the furthest potential distance from a zombie
			move = 'r';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX() - 2, getY()) > moveDistance) //checks left
		{
			moveDistance = getWorld()->distanceFromZombie(getX() - 2, getY()); //sets to the furthest potential distance from a zombie
			move = 'l';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX(), getY() + 2) > moveDistance) //checks up
		{
			moveDistance = getWorld()->distanceFromZombie(getX(), getY() + 2); //sets to the furthest potential distance from a zombie
			move = 'u';
		}
		if (getWorld()->canMove(this, getX() + 2, getY()) && getWorld()->distanceFromZombie(getX(), getY() - 2) > moveDistance) //checks down
		{
			moveDistance = getWorld()->distanceFromZombie(getX(), getY() - 2); //sets to the furthest potential distance from a zombie
			move = 'd';
		}
		if (moveDistance == dist_z) // if the current distance to a zombie was the smallest distance
			return; //the citizen does not move because it is in the "safest" spot
		else
		{
			switch (move) // move in the direction that will move it furthest from the nearest zombie, if the citizen can move to that position
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
Zombie::Zombie(double startX, double startY, StudentWorld* myWorld)
	:Actor(IID_ZOMBIE, startX, startY, right, 0, myWorld)
{
	m_paralysis = 0;
	m_movement = 0;
}
bool Zombie::canBlock()
{
	return true;
}
void Zombie::frontCoord(double &xPos, double &yPos, Direction dir) //determines position of the zombie one spot forward in the direction it faces
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
	if (m_paralysis % 2 == 0)	//zombies only move every other tick
	{
		double xPos = getX();
		double yPos = getY();
		frontCoord(xPos, yPos, getDirection());
		if (getWorld()->ifPersonInFront(xPos, yPos) && randInt(1, 3) == 3)
		{
			getWorld()->addActor('v', xPos, yPos, getDirection(), getWorld());	// if there is a person in front of the zombie, vomit on them
			getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			return;
		}
		if (m_movement == 0)	//set a new movement plan
		{
			m_movement = randInt(3, 10);	//movement plan
			if (!differentMovements())	//if it is a dumb zombie or the nearest person is more than 80 pixels away from a smart zombie
			{
				int temp = randInt(1, 4);
				switch (temp)	//choose a random dir to face
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
				differentMovements();	//there is a person 80 pixels or less from the zombie
			}
		}
		else
		{
			int temp = getDirection();
			switch (temp)	//moves in the dir it is facing if it will not be blocked, if it is blocked, set a new movement plan for next tick
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
DumbZombie::DumbZombie(double startX, double startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{
}
void DumbZombie::death() //what a dumb zombie should do when it is killed by a pit or flame
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	if (randInt(1, 10) == 1)	//one in ten chance
	{
		int dir = randInt(1, 4);	//it will chuck a vaccine in a random direction next to it when it dies
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
SmartZombie::SmartZombie(double startX, double startY, StudentWorld* myWorld)
	:Zombie(startX, startY, myWorld)
{

}
void SmartZombie::death()	//what a smart zombie does when it dies
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(2000);
}
bool SmartZombie::differentMovements()
{
	double xCoor = 0;
	double yCoor = 0;
	double p_dist = getWorld()->distanceFromPene(getX(), getY()); // find the distance to penelope
	double c_dist = getWorld()->distanceFromPerson(getX(), getY(),xCoor,yCoor); //find the distance to the nearest citizen
	if (c_dist < p_dist)	//if the distance to a citizen is closer
		p_dist = c_dist;	//set p_dist to it
	if (p_dist > 80)	//if the distance to the nearest person is larger than 80
		return false;	//do what a dumb zombie does and choose a random dir in doSomething
	if (p_dist != c_dist)	//if penelope is closer
	{
		char row = getWorld()->rowMoveToP(this);
		char col = getWorld()->colMoveToP(this);
		char otherRow = ' ';
		char otherCol = ' ';
		switch (row)
		{
		case 'r':					//if zombie and penelope are on the same row and penelope is to the right
			setDirection(right);
			return true;
			break;
		case'l':
			setDirection(left);	//if zombie and penelope are on the same row and penelop is to the left
			return true;
			break;
		case'd':
			otherRow = 'd';		//penelope is relatively to the right
			break;
		case 'a':
			otherRow = 'a';	//penelope is relatively to the left
			break;
		}
		switch (col)
		{
		case 'u':
			setDirection(up);	//if zombie and penelope are on the same col and penelope is above
			return true;	
			break;
		case'b':
			setDirection(down);	//if zombie and penelope are on the same col and penelope is below
			return true;
			break;
		case 'w':
			otherCol = 'w';		//penelope is relatatively above
			break;
		case 's':
			otherCol = 's';	//penelope is relatively below
			break;
		}
		char newDir = ' ';
		if (randInt(1, 2) == 1)	//chose randomly between a vertical or horizontal direction that will get zombie closer to penelope
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
	else	//moves closer to a citizen
	{
		if (xCoor == getX())	//if they are on the same column
		{
			if (yCoor >= getY())	//and the citizen is above the zombie
			{
				setDirection(up);
				return true;
			}
			else
			{
				setDirection(down);	//and the citizen is below the zombie
				return true;
			}
		}
		if (yCoor == getY())	//if they are on the same row
		{
			if (xCoor > getX())	
			{
				setDirection(right); //and the citizen is to the right of the zombie
				return true;
			}
			else
			{
				setDirection(left);	//and the citizen is to the left of the zombie
				return true;
			}			
		}
		// if it gets here that means the zombie and citizen were not on the same row or col
		int choose = randInt(1, 2);	//choose a random vertical or horizontal direction that will get the zombie closer to the citizen
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
	return false;
}
////////////////LANDMINE IMPLEMENTATIONS////////////////////
Landmine::Landmine(double startX, double startY, StudentWorld* myWorld)
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
	if (safety > 0)	//if the safety is on then don't do this
	{
		safety--;
		if (safety == 0)
		{
			active = true;	//set it to active
			return;
		}
	}
	if (active && getWorld()->stepOnLandmine(getX(), getY())) // if it is active and someone stepped on it
		setOffLandmine();
};
void Landmine::death() // if the landmine was overlapped with fire
{
	setDead();
	setOffLandmine();
}
void Landmine::setOffLandmine()
{
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE); 
	//creates the fire in appropriate spots with respect to the dead landmine
	for(double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY() + SPRITE_HEIGHT, up, getWorld());	//row 1 of fire
	for (double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY(), up, getWorld());   //row 2 of fire
	for (double i = 0; i < 3; i++)
		getWorld()->addActor('f', getX() - SPRITE_WIDTH + (i*SPRITE_WIDTH), getY() -SPRITE_HEIGHT, up, getWorld());	//row 3 of fire
	getWorld()->addActor('p', getX(), getY(), right, getWorld()); //creates a pit in place of the dead landmine
	setDead();
	active = false;
}
////////////////PIT IMPLEMENTATIONS////////////////////
Pit::Pit(double startX, double startY, StudentWorld* myWorld)
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
	getWorld()->dieByPitOrFlame(getX(), getY()); //checks if anything overlaps with it
};
////////////////FLAME IMPLEMENTATIONS////////////////////
Flame::Flame(double startX, double startY, Direction startDirection, StudentWorld* myWorld)
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
	if (m_lives == 0)	//if two ticks passed
	{
		setDead();
		return;
	}
	getWorld()->dieByPitOrFlame(getX(), getY());	//checks if anything overlaps with it
};
////////////////VOMIT IMPLEMENTATIONS////////////////////
Vomit::Vomit(double startX, double startY, Direction startDirection, StudentWorld* myWorld)
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
	getWorld()->infect(getX(), getY()); // infect people if there are any overlapping it
};
////////////////GOODIES IMPLEMENTATIONS////////////////////
Goodies::Goodies(const int imageID, double startX, double startY, StudentWorld* myWorld)
	: Actor(imageID, startX, startY, right, 1, myWorld)
{

}
void Goodies::doSomething() 
{
	if (isDead())
		return;
	if (getWorld()->overlapsPene(getX(), getY()))	//if penelope overlapped with a goodie
	{
		getWorld()->increaseScore(50);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		addCount();	//increases penelopes count for whichever goodie respectively
	}
};
VaccineGoodies::VaccineGoodies(int const imageID, double startX, double startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void VaccineGoodies::addCount() 
{
	getWorld()->increaseCount(1, 'v');	
};
GasCanGoodies::GasCanGoodies(int const imageID, double startX, double startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{

}
void GasCanGoodies::addCount() 
{
	getWorld()->increaseCount(5, 'f');
};
LandmineGoodies::LandmineGoodies(int const imageID, double startX, double startY, StudentWorld* myWorld)
	: Goodies(imageID, startX, startY, myWorld)
{
}
void LandmineGoodies::addCount() 
{
	getWorld()->increaseCount(2, 'l');
}
////////////////WALL IMPLEMENTATIONS////////////////////
Wall::Wall(double startX, double startY, StudentWorld* myWorld)
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
Exit::Exit(double startX, double startY, StudentWorld* myWorld)
	:Actor(IID_EXIT, startX, startY, right, 1, myWorld)
{

}
bool Exit::affectedByFlame()
{
	return false;
}
void Exit::doSomething() 
{		
	if (getWorld()->citizenCount() == 0)		//if there are no citizens left
	{
		getWorld()->canLeave(getX(), getY());	//tell penelope it can leave if she is overlapping the exit
	}
	if (getWorld()->left(getX(), getY())) //if a person is overlapping the exit and left via the exit
	{
		getWorld()->increaseScore(500);
		getWorld()->playSound(SOUND_CITIZEN_SAVED);
	}	
};