#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>

class GraphObject;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}
    
    int random(int min, int max);
    
    void moveTunnelMan();
    
    bool checkTunnelMan(int x, int y, double distance);
    
    bool isFacingTunnelMan(int x, int y, Actor::Direction dir);
    
    void yellAtTunnelMan();
    
    bool clearPathToTunnelMan(int x, int y, Actor::Direction& dir);
    
    void oilPickedUp();
    
    void sonarPickedUp();
    
    void waterPoolPickedUp();
    
    void goldPickedUp();
    
    bool goldBribed(int x, int y);
    
    bool checkSupport(int x, int y);
    
    bool isBonked(int x, int y);
    
    bool isSquirted(int x, int y);
    
    bool protesterTryToMove(int x, int y, Actor::Direction dir);

    virtual int init();

    virtual int move();

    virtual void cleanUp();

private:
    void setDisplayText();
    bool isInRange(int x, int y, double dist);
    bool isEmptySpace(int x, int y);
    bool checkDistance(int x1, int y1, int x2, int y2, double distance);
    void addBoulder(int num);
    void addGoodies(int numGold, int numBarrel);
    void addSonarKit();
    void addWaterPool();
    
    TunnelMan* m_player;
    std::list<Actor*> m_actors;
    Earth*** m_earths;
    int m_numOil;
    int m_numGold;
    int m_numBoulder;
    int m_goodieChance;
};

#endif // STUDENTWORLD_H_
