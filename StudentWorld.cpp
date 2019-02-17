#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <list>
#include "Actor.h"
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	m_penelope = nullptr;
}

/*Sets the stage with all the actors in the correct places*/
int StudentWorld::init() 
{
	string myLevel = to_string(getLevel());			//stores the level into myLevel
	string levelFile = "level0" + myLevel +".txt"; 
	Level lev(assetPath());
	Level::LoadResult result = lev.loadLevel(levelFile);		//loads the correct level file
	if (result == Level::load_fail_file_not_found)   
		cerr << "Could not find level data file" << endl;  
	else if (result == Level::load_fail_bad_format)   
		cerr << "Your level was improperly formatted" << endl;  
	else if (result == Level::load_success)
	{
	Actor* addMe;								//pointer to dynamically add actors
	cerr << "Successfully loaded level" << endl;
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
			case Level::smart_zombie:
				addMe = new SmartZombie(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a smart zombie at the position indicated by the level txt file 
				m_myActors.push_back(addMe);
				break;
			case Level::dumb_zombie:
				addMe = new DumbZombie(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			// dynamically allocate a dumb zombie at the position indicated by the level txt file 
				break;
			case Level::player:
				m_penelope = new Penelope(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a penelope at the position indicated by the level txt file 
				m_myActors.push_back(addMe);
				break;
			case Level::exit:
				addMe = new Exit(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate an exit at the position indicated by the level txt file 
				m_myActors.push_back(addMe);
				break;
			case Level::wall:
				addMe = new Wall(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a wall at the position indicated by the level txt file
				m_myActors.push_back(addMe);
				break;
			case Level::pit:
				addMe = new Pit(SPRITE_WIDTH * col, SPRITE_HEIGHT*row, this);			//dynamically allocate a pit at the position indicated by the level txt file 
				m_myActors.push_back(addMe);
				break;
			}
		}
	}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	list<Actor*>::iterator it = m_myActors.begin();	//iterator for each item in m_myActors
	m_penelope->doSomething();	//calls penelope's do something
	while (it != m_myActors.end())
	{
		if (!(*it)->isDead())		//if the actor is not dead
			(*it)->doSomething();
		it++;
	}
	if (m_penelope->isDead())
		return GWSTATUS_PLAYER_DIED;
	/*if (check if they all exited)	//figure out how to implement
		return GWSTATUS_FINISHED_LEVEL;
		*/
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
	/*!!!!!!!!!!!!!!!UPDATE THE STATUS TEXT!!!!!!!!!!!!!!!!*/
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	list<Actor*>::iterator it = m_myActors.begin();
	delete m_penelope;
	m_penelope = nullptr;
	while (it != m_myActors.end())		//frees up the list of m_myActors
	{
		delete (*it);
		*it = nullptr;
		it = m_myActors.erase(it);
	}
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}
/*checks if an object can move to the new position*/
bool StudentWorld::canMove(int xPos, int yPos)		
{
	list<Actor*>::iterator it = m_myActors.begin();
	while (it != m_myActors.end())
	{	//checks if an actor that can block is standing at the place another actor wants to move to
		if (((*it)->getX() <= xPos+SPRITE_WIDTH-1)&& 
			((*it)->getX()+SPRITE_WIDTH-1 >= xPos) &&
			((*it)->getY() <= yPos + SPRITE_HEIGHT - 1) &&
			((*it)->getY()+ SPRITE_HEIGHT-1 >= yPos)&&
			!((*it)->isDead()) && 
			((*it)->canBlock()))
		{
			return false;
		}
		it++;
	}
	return true;
}
