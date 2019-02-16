#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool canMove(int col, int row);
	//do i create a destructor by myself
	~StudentWorld();

private:
	std::list<Actor*> m_myActors;
	Actor* m_penelope;
};

#endif // STUDENTWORLD_H_
