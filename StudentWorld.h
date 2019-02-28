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
	bool blockFlame(double xPos, double yPos);		//checks if a flame can be created in that direction
	void canLeave(double xPos, double yPos);		//checks if penelope can leave
	bool left(double xPos, double yPos);			//checks if a citizen left the game							
	bool dieByPitOrFlame(double xPos, double yPos);
	bool stepOnLandmine(double xPos, double yPos);
	void infect(double xPos, double yPos);
	bool ifPersonInFront(double xPos, double yPos);
	double distance(Actor* one, double xPos, double yPos);
	double distanceFromPene(double xPos, double yPos);
	double distanceFromZombie(double xPos, double yPos);
	double distanceFromPerson(double xPos, double yPos, double& xCoor, double& yCoor);
	char rowMoveToP(Actor* two);
	char colMoveToP(Actor* two);
	char rowMoveToC(Actor* two);
	char colMoveToC(Actor* two);
	~StudentWorld();

private:
	std::list<Actor*> m_myActors;
	Penelope* m_penelope;
	int m_numCit;
};

#endif // STUDENTWORLD_H_
