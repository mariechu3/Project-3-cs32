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
	bool isDead();		//checks if actor is dead
	void setDead();			//sets Actor to dead
	virtual bool canBlock();	//checks if actor can block others
	virtual bool isPerson();	//checks if actor is a person
	virtual bool isZombie();	//checks if actor is a zombie
	virtual bool affectedByFlame();	//checks if actor can be damaged by Flame
	virtual bool blockFlame();	//checks if it can block a flame
	virtual void setInfected(bool yes) {}	//sets a person's infected status to true (overridden in person class)
	virtual void death();	//determines what to do if a person dies by flames or pit
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
	void addInfection();	//increases infection count by 1
	void setInfectionBack();	//sets infection count to 0
	virtual bool infected();	//checks if a person is infected
	int infectionCount();
	virtual void doSomething() = 0;	
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
	void addNumLand(int add);	//increases her landmine count
	int numLandmine();	//return her landmine count
	void addNumFlames(int add);	//increases her flame count
	int numFlames();	//returns her flame count
	void addNumVaccine(int add);	//increases her vaccine count
	int numVaccine();	//returns her vaccine count
	void setCompletion(); //sets her status of level completion to true
	bool completion();	 //checks if she completed the level
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
	void frontCoord(double &xPos, double &yPos, Direction dir); //checks what the planned position of the zombie would be
	virtual bool isZombie();
	virtual bool differentMovements() = 0; // different implementation for smart and dumb zombies
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
	void setOffLandmine();	//triggers the landmine/creates flames and pit/etc.
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
	virtual void addCount() = 0;		//does different actions for different goodies
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
