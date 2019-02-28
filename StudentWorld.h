#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include <list>
class Actor;
class Penelope;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	std::string updateStatusText();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void increaseCount(int add, char type);			//increases the count of goodies penelope obtained
	int citizenCount();		//checks how many citizens are left
	void addActor(char type, int startX, int startY, Direction dir, StudentWorld* myWorld);		//creates a new actor in the world
	bool touching(Actor* a1, double xPos, double yPos);		//checks if objects are touching
	bool overlapsPene(double xPos, double yPos);			//checks if object overlaps penelope
	bool overlaps(Actor* with, double xPos, double yPos);	//check if objects overlap
	bool canMove(Actor *src, double xPos, double yPos);		//checks if object can move onto new location
	bool canFlingVaccine(Actor* src, double xPos, double yPos); //checks if zombie can fling a vaccine where it wants
	bool blockFlame(double xPos, double yPos);		//checks if a flame can be created in that direction
	void canLeave(double xPos, double yPos);		//checks if penelope can leave
	bool left(double xPos, double yPos);			//checks if a citizen left the game							
	void dieByPitOrFlame(double xPos, double yPos); //kills whatever is overlapping with a pit or flame
	bool stepOnLandmine(double xPos, double yPos);	//triggers the landmine
	void infect(double xPos, double yPos);	//infects a person that is vomitted on
	bool ifPersonInFront(double xPos, double yPos);	//checks if there is a person in front to vomit on
	double distance(Actor* one, double xPos, double yPos);	//helper function for the distance from one thing to another
	double distanceFromPene(double xPos, double yPos);	//distance from penelope to the other object
	double distanceFromZombie(double xPos, double yPos);	//distance from zombie
	double distanceFromPerson(double xPos, double yPos, double& xCoor, double& yCoor); // distance from a citizen
	char rowMoveToP(Actor* two);		//move towards penelope(in the row dir)
	char colMoveToP(Actor* two);		//move towards penelope(in the col dir)
	char rowMoveToC(Actor* two);		//move towards citizen (in the row dir)
	char colMoveToC(Actor* two);		//move towards citizen (in the col dir)
	~StudentWorld();

private:
	std::list<Actor*> m_myActors;
	Penelope* m_penelope;
	int m_numCit;
};

#endif // STUDENTWORLD_H_
