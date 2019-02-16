#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor :public GraphObject
{
public:
	Actor(const int imageID, int startX, int startY, Direction startDirection, int depth, bool block, StudentWorld* myWorld);
	virtual void doSomething() = 0;
	bool isDead();
	void setDead();
	bool canBlock();
	StudentWorld* getWorld();
private:
	bool m_dead;
	StudentWorld* m_world;
	bool m_block;			//can use this to see if it overlaps
};
class Penelope : public Actor
{
public:
	Penelope(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
	int m_landmine;
	int m_flamethrower;
	int m_vaccine;
	bool m_infected;
	int m_infectionCount;
};
class Zombie : public Actor
{
public:
	Zombie(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class DumbZombie : public Zombie
{
public:
	DumbZombie(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class SmartZombie : public Zombie
{
public:
	SmartZombie(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Landmine : public Actor
{
public:
	Landmine(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Pit : public Actor
{
public:
	Pit(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Flame : public Actor
{
public:
	Flame(int startX, int startY, Direction startDirection, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Vomit : public Actor
{
public:
	Vomit(int startX, int startY, Direction startDirection, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Goodies : public Actor
{
public:
	Goodies(const int imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class VaccineGoodies : public Goodies
{
public:
	VaccineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class GasCanGoodies : public Goodies
{
public:
	GasCanGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class LandmineGoodies : public Goodies
{
public:
	LandmineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
class Exit : public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
#endif // ACTOR_H_
