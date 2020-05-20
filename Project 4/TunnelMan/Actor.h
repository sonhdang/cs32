#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);
    virtual void doSomething();
    virtual bool isProtester()
    {
        return false;
    }
    
    virtual bool isBonkable()
    {
        return false;
    }
    
    virtual bool isBoulder()
    {
        return false;
    }
    
    virtual void doDamage(int amount)
    {
        return;
    }
    
    bool isAlive()
    {
        return m_alive;
    }
    void setAlive(bool state)
    {
        m_alive = state;
    }
    StudentWorld* getStudentWorld()
    {
        return m_studentWorld;
    }
    virtual ~Actor(){}
    
private:
    bool m_alive;
    StudentWorld* m_studentWorld;
};

class Earth : public Actor
{
public:
    Earth(int startX, int startY, StudentWorld* studentWorld):Actor(TID_EARTH, startX, startY, right, .25, 3, studentWorld) {};
};

class Human : public Actor
{
public:
    Human(int imageID, int startX, int startY, Direction dir, StudentWorld* studentWorld, int hp);
    int getHealth()
    {
        return m_hp;
    }
    virtual void doDamage(int howMuch)
    {
        m_hp -= howMuch;
    }
    bool isBonkable()
    {
        return true;
    }
    
private:
    int m_hp;
};

class TunnelMan : public Human
{
public:
    TunnelMan(StudentWorld* studentWorld);
    virtual void doSomething();
    
    int getWater()
    {
        return m_water;
    }
    int getSonar()
    {
        return m_sonar;
    }
    int getGold()
    {
        return m_gold;
    }
    
    void setWater(int num)
    {
        m_water = num;
    }
    void setSonar(int num)
    {
        m_sonar = num;
    }
    void setGold(int num)
    {
        m_gold = num;
    }
private:
    int m_water;
    int m_sonar;
    int m_gold;
           //pointer to studentWorld to access other graph ojects
};

class Protester : public Human
{
public:
    Protester(int imageID, StudentWorld* studentWorld, int hp);
    
    void doDamage(int howMuch);
    
    bool isLeaveTheField()
    {
        return m_leaveTheField;
    }
    
    void leaveTheField()
    {
        m_leaveTheField = true;
    }
    
    int getWaitingTicks()
    {
        return m_waitingTicks;
    }
    
    int getWaitingTicksTracking()
    {
        return m_waitingTicksTracking;
    }
    
    void incWaitingTicksTracking()
    {
        m_waitingTicksTracking++;
    }
    
    int getRestingTicks()
    {
        return m_restingTicks;
    }
    
    int getRestingTicksTracking()
    {
        return m_restingTicksTracking;
    }
    
    void incRestingTicksTracking()
    {
        m_restingTicksTracking++;
    }
    
    void resetRestingTicksTracking()
    {
        m_restingTicksTracking = 0;
    }
    
    int getNumSquaresToMove()
    {
        return m_numSquaresToMoveInCurDirection;
    }
    
    void setNumSquaresToMove(int num)
    {
        m_numSquaresToMoveInCurDirection = num;
    }
    
    void decNumSquaresToMove()
    {
        m_numSquaresToMoveInCurDirection--;
    }
    
    int getPerpendicularTicks()
    {
        return m_perpendicularTicks;
    }
    
    void setPerpendicularTicks(int num)
    {
        m_perpendicularTicks = num;
    }
    
    void resetPerpendicularTicks(Direction dir);
    
    void moveInDirection(Direction dir);
    
    bool isStunned()
    {
        return m_stunnedTicksTracking < m_stunnedTicks;
    }
    
    void incStunnedTicksTracking()
    {
        m_stunnedTicksTracking++;
    }
    
    void resetStunnedTicksTracking()
    {
        m_stunnedTicksTracking = 0;
    }
    
    bool isProtester()
    {
        return true;
    }
    bool isBonkable()
    {
        return true;
    }
    
private:
    int m_waitingTicks;
    int m_waitingTicksTracking;
    int m_restingTicks;             //after yell at the TunnelMan
    int m_restingTicksTracking;
    int m_perpendicularTicks;
    int m_stunnedTicks;
    int m_stunnedTicksTracking;
    int m_numSquaresToMoveInCurDirection;
    bool m_leaveTheField;
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* studentWorld);
    virtual void doSomething();
};

class PickableObject: public Actor
{
public:
    PickableObject(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, bool stableState, int time);
    
    virtual void doSomething() = 0;
    
    bool isStable()
    {
        return m_stable;
    }
    int getTime()
    {
        return m_time;
    }
    
    void setStable(bool stableState)
    {
        m_stable = stableState;
    }
    
    void setTime(int time)
    {
        m_time = time;
    }
    
    void decTime()
    {
        m_time--;
    }
    
    bool isPickedUp();
    
    void tick();
    
private:
    bool m_stable;
    int m_time;
};

class Barrel : public PickableObject
{
public:
    Barrel(int startX, int startY, StudentWorld* studentWorld);
    void doSomething();
};

class SonarKit : public PickableObject
{
public:
    SonarKit(int startX, int startY, StudentWorld* studentWorld);
    void doSomething();
};

class WaterPool : public PickableObject
{
public:
    WaterPool(int startX, int startY, StudentWorld* studentWorld);
    void doSomething();
};

class GoldNugget : public PickableObject
{
public:
    GoldNugget(int startX, int startY, StudentWorld* studentWorld, bool stableState);
    void doSomething();
};

class DamageObject : public Actor
{
public:
    DamageObject(int imageID, int startX, int starY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, int move);
    
    int getMove()
    {
        return m_move;
    }
    
    void decMove()
    {
        m_move--;
    }
    
    
private:
    int m_move;
};

class Boulder : public DamageObject
{
public:
    
    enum State { stable, waiting, falling };
    
    Boulder(int startX, int startY, StudentWorld* studentWorld);
    void doSomething();
    bool isBoulder()
    {
        return true;
    }
private:
    int m_time;
    State m_state;
};

class Squirt : public DamageObject
{
public:
    Squirt(int startX, int startY, Direction dir, StudentWorld* studentWorld);
    void doSomething();
    
};
#endif // ACTOR_H_
