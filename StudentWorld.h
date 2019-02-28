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
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	std::string updateStatusText();
	bool canMove(Actor *src, double xPos, double yPos);				//checks if object can move onto new location
	bool touching(Actor* a1, double xPos, double yPos);		//checks if objects are touching
	bool overlapsPene(double xPos, double yPos);			//checks if object overlaps penelope
	bool overlaps(Actor* with, double xPos, double yPos);	//check if objects overlap
	bool blockFlame(double xPos, double yPos);		//checks if a flame can be created in that direction
	void canLeave(double xPos, double yPos);		//checks if penelope can leave
	bool left(double xPos, double yPos);			//checks if a citizen left the game
	void increaseCount(int add, char type);			//increases the count of goodies penelope obtained
	int citizenCount();								//checks how many citizens are left
	void addActor(char type, int startX, int startY, Direction dir, StudentWorld* myWorld);		//creates a new actor in the world
	bool dieByPitOrFlame(double xPos, double yPos);
	bool stepOnLandmine(double xPos, double yPos);
	void infect(double xPos, double yPos);
	bool ifPersonInFront(double xPos, double yPos);
	double distance(Actor* one, Actor*two);
	double distanceFromPene(Actor* two);
	double distanceFromZombie(Actor* two);
	int sameRowAsP(Actor* two);
	int sameColAsP(Actor* two);
	~StudentWorld();

private:
	std::list<Actor*> m_myActors;
	Penelope* m_penelope;
	int m_numCit;
};

#endif // STUDENTWORLD_H_
