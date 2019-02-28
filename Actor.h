#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor :public GraphObject
{
public:
	Actor(const int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* myWorld);
	virtual void doSomething() = 0;
	bool isDead();
	void setDead();
	virtual bool canBlock();
	virtual bool isPerson();
	virtual bool isZombie();
	virtual bool affectedByFlame();
	virtual bool affectedByVomit();
	//virtual void setOffLandmine(){}
	virtual bool blockFlame();
	virtual void setInfected(bool yes) {}
	virtual void death();
	StudentWorld* getWorld();
private:
	bool m_dead;
	StudentWorld* m_world;
};
class Person : public Actor
{
public:
	Person(const int imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void setInfected(bool yes);
	void addInfection();
	void setInfectionBack();
	virtual bool infected();
	int infectionCount();
	virtual void doSomething() = 0;	
	virtual bool affectedByVomit();
	virtual bool isPerson();
	virtual bool canBlock();
private:
	bool m_infected;
	int m_infectionCount;
};
class Penelope : public Person
{
public:
	Penelope(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	void addNumLand(int add);
	int numLandmine();
	void addNumFlames(int add);
	int numFlames();
	void addNumVaccine(int add);
	int numVaccine();
	void setCompletion();
	bool completion();
private:
	int m_landmine;
	int m_flamethrower;
	int m_vaccine;
	bool m_completed;
};
class Citizen : public Person
{
public:
	Citizen(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	virtual void setInfected(bool yes);
	virtual void death();
private:
	int m_paralysis;
	bool firstInfection;
};
class Zombie : public Actor
{
public:
	Zombie(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	void frontCoord(double &xPos, double &yPos, Direction dir);
	virtual bool isZombie();
	virtual bool differentMovements() = 0;
	virtual bool canBlock();
private:
	int m_paralysis;
	int m_movement;
};
class DumbZombie : public Zombie
{
public:
	DumbZombie(int startX, int startY, StudentWorld* myWorld);
	virtual bool differentMovements();
	virtual void death();
private:
};
class SmartZombie : public Zombie
{
public:
	SmartZombie(int startX, int startY, StudentWorld* myWorld);
	virtual bool differentMovements();
	virtual void death();
private:
};
class Landmine : public Actor
{
public:
	Landmine(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	void setOffLandmine();
	virtual void death();
private:
	bool active;
	int safety;
	bool setOff;
};
class Pit : public Actor
{
public:
	Pit(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	virtual bool affectedByFlame();
	virtual bool blockFlame();
private:
};
class Flame : public Actor
{
public:
	Flame(int startX, int startY, Direction startDirection, StudentWorld* myWorld);
	virtual void doSomething();
	virtual bool affectedByFlame();
private:
	int m_lives;
};
class Vomit : public Actor
{
public:
	Vomit(int startX, int startY, Direction startDirection, StudentWorld* myWorld);
	virtual void doSomething();
	virtual bool affectedByFlame();
private:
	int m_lives;
};
class Goodies : public Actor
{
public:
	Goodies(const int imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	virtual void addCount() = 0;
private:
};
class VaccineGoodies : public Goodies
{
public:
	VaccineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void addCount();
private:
};
class GasCanGoodies : public Goodies
{
public:
	GasCanGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void addCount();
private:
};
class LandmineGoodies : public Goodies
{
public:
	LandmineGoodies(int const imageID, int startX, int startY, StudentWorld* myWorld);
	virtual void addCount();
private:
};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();
	virtual bool canBlock();
	virtual bool affectedByFlame();
private:
};
class Exit : public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* myWorld);
	virtual void doSomething();	
	virtual bool affectedByFlame();
private:
};
#endif // ACTOR_H_
