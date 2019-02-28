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
	Level lev(assetPath());
	Level::LoadResult result = lev.loadLevel(levelFile);		//loads the correct level file
	if (getLevel() == 99 || result == Level::load_fail_file_not_found)
	{
		cerr << "Could not find level data file/player won" << endl;
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted" << endl;
		return GWSTATUS_LEVEL_ERROR;
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
				/*UPDATE DEPTH AND DIR OF ZOMBIES WHEN SPECS ARE UPDATED!*/
			case Level::citizen:
				addMe = new Citizen(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a smart zombie at the position indicated by the level txt file 
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
	return GWSTATUS_CONTINUE_GAME;
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
		return GWSTATUS_PLAYER_DIED;
	if (m_penelope->completion())
	{
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;
	}
	it = m_myActors.begin();
	while (it != m_myActors.end())			//cleans up all actors that died that round
	{
		if ((*it)->isDead())
		{
			delete (*it);
			it = m_myActors.erase(it);
			continue;
		}
		it++;
	}
	setGameStatText(updateStatusText());
	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::ifPersonInFront(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if ((*it)->affectedByVomit() && overlaps((*it), xPos, yPos))
		{
			return true;
		}
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos))
	{
		return true;
	}
	return false;
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
		oss.fill(' ');
		oss << "Score: " << setw(6)<< getScore() << "  ";
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
	/*implement if they were to run into penelope*/
	if (src != m_penelope && touching(m_penelope, xPos, yPos) && !m_penelope->isDead())
		return false;
	return true;
}
bool StudentWorld::touching(Actor* a1, double xPos, double yPos)
{
	if ((a1->getX() <= xPos + SPRITE_WIDTH - 1) &&
		(a1->getX() + SPRITE_WIDTH - 1 >= xPos) &&
		(a1->getY() <= yPos + SPRITE_HEIGHT - 1) &&
		(a1->getY() + SPRITE_HEIGHT - 1 >= yPos))
		return true;
	return false;
}
bool StudentWorld::left(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if ((*it)->isPerson() && touching((*it), xPos, yPos))
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
	if (touching(m_penelope,xPos, yPos))
		m_penelope->setCompletion();
}

int StudentWorld::citizenCount()
{
	return m_numCit;
}
bool StudentWorld::overlapsPene(double xPos, double yPos)
{
	if (overlaps(m_penelope, xPos, yPos))
		return true;
	return false;
}
bool StudentWorld::overlaps(Actor* with, double xPos, double yPos)
{
	if (distance(with,xPos,yPos) <= 10)
		return true;
	return false;
}
bool StudentWorld::blockFlame(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (!(*it)->affectedByFlame() && overlaps((*it), xPos, yPos))
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
		if ((*it)->affectedByVomit() && overlaps((*it), xPos, yPos))
		{
			(*it)->setInfected(true);
		}
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos))
	{
		m_penelope->setInfected(true);
	}
}
double StudentWorld::distance(Actor* one, double xPos, double yPos)
{
	double x1 = one->getX() + ((SPRITE_WIDTH - 1) / 2);
	double y1 = one->getY() + ((SPRITE_HEIGHT - 1) / 2);
	double x2 = xPos + ((SPRITE_WIDTH - 1) / 2);
	double y2 = yPos + ((SPRITE_HEIGHT - 1) / 2);
	double dx = x1 - x2;
	double dy = y1 - y2;
	return sqrt((dx*dx) + (dy*dy));
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
		if ((*it)->isZombie() && distance((*it), xPos, yPos) < min)
			min = distance(*it, xPos, yPos);
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
		if ((*it)->isPerson() && distance((*it), xPos, yPos) < min)
		{
			min = distance(*it, xPos, yPos);
			xCoor = (*it)->getX();
			yCoor = (*it)->getY();
		}
		it++;
	}
	return min;
}
bool StudentWorld::stepOnLandmine(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if (((*it)->isPerson() || (*it)->isZombie()) && overlaps((*it), xPos, yPos) && !(*it)->isDead())
			return true;
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos))
		return true;
	return false;
}
bool StudentWorld::dieByPitOrFlame(double xPos, double yPos)
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{
		if ((*it)->affectedByFlame() && overlaps((*it), xPos, yPos) && !(*it)->isDead())
		{
			(*it)->setDead();
			(*it)->setOffLandmine(); // will call the base version of setOffLandmine if it is not a landmine and do nothing
			if ((*it)->isPerson())
			{
				increaseScore(-1000);
				playSound(SOUND_CITIZEN_DIE);
			}
			else if ((*it)->isZombie())
			{
				increaseScore(1000);
				playSound(SOUND_ZOMBIE_DIE);
			}
			/*
			else if(dumbzombie)
				increaseScore(1000);
			else if (smart)
				increaseScore(2000);
				play dying sound
			*/
			//return true;
		}
		it++;
	}
	if (overlaps(m_penelope, xPos, yPos))
	{
		if (!m_penelope->isDead())
		{
			m_penelope->setDead();
			playSound(SOUND_PLAYER_DIE);
			decLives();
		}
		//return true;
	}
	return false;
}
void StudentWorld::addActor(char type, int startX, int startY, Direction dir, StudentWorld* myWorld)
{
	Actor* addme;
	switch (type)
	{
	case 'f':
		addme = new Flame(startX, startY, dir, myWorld);
		m_myActors.push_front(addme);
		break;
	case 'l':
		addme = new Landmine(startX, startY, myWorld);
		m_myActors.push_front(addme);
		break;
	case 'p':
		addme = new Pit(startX, startY, myWorld);
		m_myActors.push_front(addme);
		break;
	case 'v':
		addme = new Vomit(startX, startY, dir, myWorld);
		m_myActors.push_front(addme);
		break;
	case 'd':
		addme = new DumbZombie(startX, startY, myWorld);
		m_myActors.push_front(addme);
		break;
	case 's':
		addme = new SmartZombie(startX, startY, myWorld);
		m_myActors.push_front(addme);
		break;
	}
	
}
char StudentWorld::rowMoveToP(Actor* two)
{
	if (two->getY() == m_penelope->getY())
	{
		if (two->getX() < m_penelope->getX())
			return 'r'; // means penelope is to the right
		else
			return 'l'; // means on the left
	}
	else if (two->getX() < m_penelope->getX())	//find relative direction of where penelope is
		return 'd';
	else if (two->getX() > m_penelope->getX())
		return 'a';
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
	else if (two->getY() < m_penelope->getY())
		return 'w';
	else if (two->getY() > m_penelope->getY())
		return 's';
}
char StudentWorld::rowMoveToC(Actor* two)
{
	if (two->getY() == m_penelope->getY())
	{
		if (two->getX() < m_penelope->getX())
			return 'r'; // means penelope is to the right
		else
			return 'l'; // means on the left
	}
	else if (two->getX() < m_penelope->getX())	//find relative direction of where penelope is
		return 'd';
	else if (two->getX() > m_penelope->getX())
		return 'a';
}
char StudentWorld::colMoveToC(Actor* two)
{
	if (two->getX() == m_penelope->getX())
	{
		if (two->getY() < m_penelope->getY())
			return 'u'; // means penelope above
		else
			return 'b'; // means penelope below
	}
	else if (two->getY() < m_penelope->getY())
		return 'w';
	else if (two->getY() > m_penelope->getY())
		return 's';
}
void StudentWorld::increaseCount(int add, char type)
{
	switch (type)
	{
	case 'f':
		m_penelope->addNumFlames(add);
		break;
	case 'l':
		m_penelope->addNumLand(add);
		break;
	case 'v':
		m_penelope->addNumVaccine(add);
		break;
	}
}

