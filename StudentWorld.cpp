#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include "Level.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <list>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath),m_myActors()
{
	m_penelope = nullptr;
	m_numCit = 0;
}
/*Sets the stage with all the actors in the correct places*/
int StudentWorld::init() 
{
	string myLevel = to_string(getLevel());			//stores the level into myLevel
	string levelFile = "level0" + myLevel +".txt"; 
	//string levelFile = "level03.txt";
	Level lev(assetPath());
	Level::LoadResult result = lev.loadLevel(levelFile);		//loads the correct level file
	if (getLevel() == 99 || result == Level::load_fail_file_not_found)
	{
		cerr << "Could not find level data file/player won" << endl;
		return GWSTATUS_PLAYER_WON;					//player won the game
	}
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted" << endl;
		return GWSTATUS_LEVEL_ERROR;	//file improperly formatted
	}
	else if (result == Level::load_success)
	{
	Actor* addMe;								//pointer to dynamically add actors
	cerr << "Successfully loaded level0"+myLevel+".txt" << endl;
	Level::MazeEntry ge;
	for (int col = 0; col < 16; col++)		//goes through each position in the level data files
	{
		for (int row = 0; row < 16; row++)
		{
			ge = lev.getContentsOf(col, row);
			switch (ge)
			{
			case Level::empty:  //do nothing
				break;
			case Level::citizen:
				addMe = new Citizen(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a citizen at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::smart_zombie:
				addMe = new SmartZombie(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a smart zombie at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::dumb_zombie:
				addMe = new DumbZombie(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);// dynamically allocate a dumb zombie at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::player:
				m_penelope = new Penelope(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a penelope at the position indicated by the level txt file 
				break;
			case Level::exit:
				addMe = new Exit(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate an exit at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::wall:
				addMe = new Wall(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a wall at the position indicated by the level txt file
				m_myActors.push_front(addMe);
				break;
			case Level::pit:
				addMe = new Pit(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a pit at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::gas_can_goodie:
				addMe = new GasCanGoodies(IID_GAS_CAN_GOODIE,SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a gas_can at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::landmine_goodie:
				addMe = new LandmineGoodies(IID_LANDMINE_GOODIE,SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a landmine_goodie at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			case Level::vaccine_goodie:
				addMe = new VaccineGoodies(IID_VACCINE_GOODIE,SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a vaccine_ at the position indicated by the level txt file 
				m_myActors.push_front(addMe);
				break;
			default:
				break;
			}
		}
	}
	}
	return GWSTATUS_CONTINUE_GAME;	//player continues the game
}
int StudentWorld::move()
{
	m_numCit = 0;
	list<Actor*>::iterator it = m_myActors.begin();	//iterator for each item in m_myActors
	m_penelope->doSomething();	//calls penelope's do something
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->isPerson())
			m_numCit++;				//checks if it is a citizen
		it++;
	}
	it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead())		//if the actor is not dead
			(*it)->doSomething();
		it++;
	}
	if (m_penelope->isDead())
		return GWSTATUS_PLAYER_DIED;	//penelope died
	if (m_penelope->completion())
	{
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;		//the level was finish
	}
	it = m_myActors.begin();
	while (it != m_myActors.end())			//cleans up all actors that died that round
	{
		if ((*it)->isDead())
		{
			delete (*it);
			it = m_myActors.erase(it);	//cleanup dead actors after each move tick
			continue;
		}
		it++;
	}
	setGameStatText(updateStatusText());	//update the status text
	return GWSTATUS_CONTINUE_GAME;
}
string StudentWorld::updateStatusText()
{
	ostringstream oss;
	if (getScore() >= 0)
	{
		oss.fill('0');
		oss << "Score: " << setw(6) << getScore() << "  ";
	}
	else
	{
		oss.fill('0');
		oss << "Score: " << '-' << setw(5)<< -getScore() << "  ";	//if the score was negative
	}

	oss << "Level: " << getLevel() << "  ";
	oss << "Lives: " << getLives() << "  ";
	oss << "Vaccines: " << m_penelope->numVaccine() << "  ";
	oss << "Flames: " << m_penelope->numFlames() << "  ";
	oss << "Mines: " << m_penelope-> numLandmine() << "  ";
	oss << "Infected: " << m_penelope->infectionCount() << "  ";
	return oss.str();
 }

void StudentWorld::cleanUp()
{
	delete m_penelope;
	m_penelope = nullptr;
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())		//frees up the list of m_myActors
	{
		delete (*it);
		it= m_myActors.erase(it);
	}
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}
void StudentWorld::addActor(char type, double startX, double startY, Direction dir, StudentWorld* myWorld)
{
	Actor* addme;
	switch (type)
	{
	case 'f':	
		addme = new Flame(startX, startY, dir, myWorld);		//creats a new flame at the specified location/dir and adds to container
		m_myActors.push_front(addme);
		break;
	case 'l':
		addme = new Landmine(startX, startY, myWorld);		//creates a new landmine at the specified location/dir and adds to container
		m_myActors.push_front(addme);
		break;
	case 'p':
		addme = new Pit(startX, startY, myWorld);	//creates a new pit at the specified location/dir and adds to container
		m_myActors.push_front(addme);
		break;
	case 'v':
		addme = new Vomit(startX, startY, dir, myWorld);	//creates a vomit at the specified location/dir and adds to container
		m_myActors.push_front(addme);
		break;
	case 'm':
		addme = new VaccineGoodies(IID_VACCINE_GOODIE, startX, startY, myWorld);	//creates a vaccineGoodie at the specified location/dir and adds to containers
		m_myActors.push_front(addme);
		break;
	case 'd':
		addme = new DumbZombie(startX, startY, myWorld);	//creates a dumb zombie at the specified location/ dir and adds to containers
		m_myActors.push_front(addme);
		break;
	case 's':
		addme = new SmartZombie(startX, startY, myWorld);	//creates a smart zombie at the specified location/dir and adds to containers
		m_myActors.push_front(addme);
		break;
	}
}
void StudentWorld::increaseCount(int add, char type)
{
	switch (type)
	{
	case 'f':
		m_penelope->addNumFlames(add);	//increases penelopes flame count
		break;
	case 'l':
		m_penelope->addNumLand(add);	//increases penelopes landmine count
		break;
	case 'v':
		m_penelope->addNumVaccine(add);	//increases penelopes vaccine count
		break;
	}
}
bool StudentWorld::touching(Actor* a1, double xPos, double yPos)	//checks if an actor is touching the specified location
{
	if ((a1->getX() <= xPos + SPRITE_WIDTH - 1) &&
		(a1->getX() + SPRITE_WIDTH - 1 >= xPos) &&
		(a1->getY() <= yPos + SPRITE_HEIGHT - 1) &&
		(a1->getY() + SPRITE_HEIGHT - 1 >= yPos))
		return true;
	return false;
}
bool StudentWorld::overlaps(Actor* with, double xPos, double yPos)	//checks if an actor overlaps with the specified location
{
	if (distance(with, xPos, yPos) <= 10)
		return true;
	return false;
}
bool StudentWorld::overlapsPene(double xPos, double yPos)	//checks if penelope overlaps with the position
{
	if (overlaps(m_penelope, xPos, yPos))
		return true;
	return false;
}
double StudentWorld::distance(Actor* one, double xPos, double yPos)	//checks the distance between an actor and the position
{
	double x1 = one->getX() + ((SPRITE_WIDTH - 1) / 2);
	double y1 = one->getY() + ((SPRITE_HEIGHT - 1) / 2);
	double x2 = xPos + ((SPRITE_WIDTH - 1) / 2);
	double y2 = yPos + ((SPRITE_HEIGHT - 1) / 2);
	double dx = x1 - x2;
	double dy = y1 - y2;
	return sqrt((dx*dx) + (dy*dy));	//returns the distance
}
double StudentWorld::distanceFromPene(double xPos, double yPos)
{
	return distance(m_penelope, xPos, yPos);
}
double StudentWorld::distanceFromZombie(double xPos, double yPos)
{
	double min = 1000;
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->isZombie() && distance((*it), xPos, yPos) < min)
			min = distance(*it, xPos, yPos);	//sets the min to the distance to the closest zombie
		it++;
	}
	return min;
}
double StudentWorld::distanceFromPerson(double xPos, double yPos, double& xCoor, double& yCoor)
{
	double min = 1000;
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->isPerson() && distance((*it), xPos, yPos) < min)
		{
			min = distance(*it, xPos, yPos);	//gets the distance to the closest person
			xCoor = (*it)->getX();	//gets the coordinates of the closest person
			yCoor = (*it)->getY();
		}
		it++;
	}
	return min;
}
/*checks if an object can move to the new position*/
bool StudentWorld::canMove(Actor* src, double xPos, double yPos)		
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{	//checks if an actor that can block is standing at the place another actor wants to move to
		if ((*it) != src && touching((*it),xPos,yPos) &&
			!((*it)->isDead()) && 
			((*it)->canBlock()))
		{
			return false;
		}
		it++;
	}
	if (src != m_penelope && touching(m_penelope, xPos, yPos) && !m_penelope->isDead())
		return false;
	return true;
}
bool StudentWorld::left(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead()&& (*it)->isPerson() && overlaps((*it), xPos, yPos))	//checks if a person left via the exit
		{
			(*it)->setDead();
			return true;
		}
		it++;
	}
	return false;
}
void StudentWorld::canLeave(double xPos, double yPos)
{
	if (overlaps(m_penelope,xPos, yPos))	//checks if penelope is allowed to leave 
		m_penelope->setCompletion();	//sets her completion to true
}
int StudentWorld::citizenCount()
{
	return m_numCit;
}
bool StudentWorld::blockFlame(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->blockFlame() && overlaps((*it), xPos, yPos))	//checks if a flame is blocked
			return true;
		it++;
	}
	return false;
}
void StudentWorld::infect(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->isPerson() && overlaps((*it), xPos, yPos))
		{
			(*it)->setInfected(true);	//infects the person
		}
		it++;
	}
	if (!m_penelope->isDead() && overlaps(m_penelope, xPos, yPos))
	{
		m_penelope->setInfected(true);	//infects penelope
	}
}
bool StudentWorld::stepOnLandmine(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (((*it)->isPerson() || (*it)->isZombie()) && overlaps((*it), xPos, yPos) && !(*it)->isDead())
			return true;	//if a person/zombie is on the landmine, it is triggered
		it++;
	}
	if (!m_penelope->isDead() && overlaps(m_penelope, xPos, yPos))
		return true;	//if penelope is on a landmine it is triggered
	return false;
}
void StudentWorld::dieByPitOrFlame(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if ((*it)->affectedByFlame() && overlaps((*it), xPos, yPos) && !(*it)->isDead())
		{
			(*it)->death();	//calls each objects method for what it does when it dies by flame/pit
		}
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos))
	{
		if (!m_penelope->isDead())
		{
			m_penelope->setDead();	//kills penelopes
			playSound(SOUND_PLAYER_DIE);
			decLives();
		}
	}
}
bool StudentWorld::ifPersonInFront(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead() && (*it)->isPerson() && overlaps((*it), xPos, yPos))
		{
			return true;	//checks if a person
		}
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos) && !m_penelope->isDead())
	{
		return true;	//or penelope is in front
	}
	return false;
}
bool StudentWorld::canFlingVaccine(Actor *src, double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if ((*it)!= src && overlaps((*it), xPos, yPos) && !(*it)->isDead())
			return false;	//if there are any objects in the specified position, return false
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos) && !m_penelope->isDead())
		return false;	//if penelope is in the specified position, return false
	return true;
}
char StudentWorld::rowMoveToP(Actor* two)
{
	if (two->getY() == m_penelope->getY())
	{
		if (two->getX() < m_penelope->getX())
			return 'r'; // means penelope is to the right
		else
			return 'l'; // means penelope is on the left
	}
	else if (two->getX() < m_penelope->getX())	//find relative direction of where penelope is (to the right)
		return 'd';
	else if (two->getX() > m_penelope->getX()) //find relative direction of where penelope is (to the left)
		return 'a';
	return ' ';
}
char StudentWorld::colMoveToP(Actor* two)
{
	if (two->getX() == m_penelope->getX())
	{
		if (two->getY() < m_penelope->getY())
			return 'u'; // means penelope above
		else
			return 'b'; // means penelope below
	}
	else if (two->getY() < m_penelope->getY())	//means penelope is relatively above
		return 'w';
	else if (two->getY() > m_penelope->getY()) //means penelope is relatively below
		return 's';
	return ' ';
}

