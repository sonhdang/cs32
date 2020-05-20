#include "Actor.h"
#include "StudentWorld.h"
#include "algorithm"

using namespace std;

/*-----------------------------------------------------------------------
                            ACTOR IMPLEMENTATION
-----------------------------------------------------------------------*/

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
: GraphObject(imageID, startX, startY, dir, size, depth), m_alive(true), m_studentWorld(studentWorld)
{
    setVisible(true);
}

void Actor::doSomething()
{
    std::cout << "What do you want me to do?";
}

/*-----------------------------------------------------------------------
                            HUMAN IMPLEMENTATION
 -----------------------------------------------------------------------*/

Human::Human(int imageID, int startX, int startY, Direction dir, StudentWorld* studentWorld, int hp):Actor(imageID, startX, startY, dir, 1.0, 0, studentWorld), m_hp(hp){}

/*-----------------------------------------------------------------------
                           TUNNELMAN IMPLEMENTATION
 -----------------------------------------------------------------------*/

TunnelMan::TunnelMan(StudentWorld* studentWorld):Human(TID_PLAYER, 30, 60, right, studentWorld, 10), m_water(5), m_sonar(1), m_gold(0){}

void TunnelMan::doSomething()
{
    if (isAlive())
    {
        if (getHealth() <= 0)
        {
            setVisible(false);
            setAlive(false);
        }
        else
            getStudentWorld()->moveTunnelMan();
    }
}

/*-----------------------------------------------------------------------
                         PROTESTER IMPLEMENTATION
 -----------------------------------------------------------------------*/

Protester::Protester(int imageID, StudentWorld* studentWorld, int hp):Human(imageID, 60, 60, left, studentWorld, hp), m_leaveTheField(false), m_restingTicks(15), m_waitingTicksTracking(0), m_restingTicksTracking(0), m_perpendicularTicks(0)
{
    const int level = getStudentWorld()->getLevel();
    m_waitingTicks = max(0, 3 - level / 4);
    m_numSquaresToMoveInCurDirection = getStudentWorld()->random(8, 60);
    m_stunnedTicks = max(50, 100 - level * 10);
    m_stunnedTicksTracking = m_stunnedTicks;
}

void Protester::doDamage(int howMuch)
{
    if (howMuch == 2)
        m_stunnedTicksTracking = 0;
    Actor::doDamage(howMuch);
}

void Protester::moveInDirection(Direction dir)
{
    int x = getX();
    int y = getY();
    switch(dir)
    {
        case up:
            if (getDirection() != up)
                setDirection(up);
            moveTo(x, y + 1);
            break;
        case down:
            if (getDirection() != down)
                setDirection(down);
            moveTo(x, y - 1);
            break;
        case left:
            if (getDirection() != left)
                setDirection(left);
            moveTo(x - 1, y);
            break;
        case right:
            if (getDirection() != right)
                setDirection(right);
            moveTo(x + 1, y);
            break;
        case none:
        default:
            return;
    }
}

void Protester::resetPerpendicularTicks(Direction dir)
{
    switch(getDirection())
    {
        case up:
        case down:
            if (dir != up || dir != down)
                m_perpendicularTicks = 0;
            else
            {
                m_perpendicularTicks++;
            }
            break;
        case left:
        case right:
            if (dir != left || dir != right)
                m_perpendicularTicks = 0;
            else
            {
                m_perpendicularTicks++;
            }
        case none:
            return;
    }
}

/*-----------------------------------------------------------------------
 R                     REGULARPROTESTER IMPLEMENTATION
 -----------------------------------------------------------------------*/

RegularProtester::RegularProtester(StudentWorld* studentWorld):Protester(TID_PROTESTER, studentWorld, 5){}

void RegularProtester::doSomething()
{
    StudentWorld* sw = getStudentWorld();
    if(isAlive() && !isLeaveTheField())
    {
        int x = getX();
        int y = getY();
        if (isLeaveTheField())
        {
            if (x == 60 && y == 60)
            {
                setVisible(false);
                setAlive(false);
            }
        }
        else
        {
            if (getHealth() <= 0)   //if Health is below 0 -> leave the field
                leaveTheField();
            else if (isStunned())
                incStunnedTicksTracking();
            else if ((getWaitingTicksTracking() % (getWaitingTicks() + 1)) != 0)
                incWaitingTicksTracking();  //waiting time
            else
            {
                Direction dir = getDirection();
                if (sw->checkTunnelMan(x, y, 4.0) && sw->isFacingTunnelMan(x, y, getDirection())) //am I close enough to yell?
                {
                    if(getRestingTicksTracking() >= getRestingTicks())
                    {
                        sw->yellAtTunnelMan();
                        resetRestingTicksTracking();
                    }
                    else
                        incWaitingTicksTracking();
                }
                else if (sw->clearPathToTunnelMan(x, y, dir))   //am I face to face with TunnelMan?
                {
                    moveInDirection(dir);
                }
                else    //I guess I wander around
                {
                    if (getNumSquaresToMove() == 0)
                    {
                        int newDir;
                        Direction tempDir = none;
                        do
                        {
                            newDir = sw->random(1, 4);
                            switch(newDir)
                            {
                                case 1:
                                    tempDir = up;
                                    break;
                                case 2:
                                    tempDir = down;
                                    break;
                                case 3:
                                    tempDir = left;
                                    break;
                                case 4:
                                    tempDir = right;
                                    break;
                            }
                            if (sw->protesterTryToMove(x, y, tempDir))
                                newDir = 0;
                        }
                        while (newDir != 0);
                        resetPerpendicularTicks(tempDir);
                        setDirection(tempDir);
                        setNumSquaresToMove(sw->random(8, 60));
                    }
                    else
                    {
                        if(sw->protesterTryToMove(x, y, dir))
                        {
                            if(getPerpendicularTicks() < 200)   //Have I changed direction after 200 ticks?
                            {
                                //resetPerpendicularTicks(dir);
                                moveInDirection(dir);    //can be replaced
                                decNumSquaresToMove();
                            }
                            else
                            {
                                int newDir = sw->random(1, 2);
                                Direction option1 = none;
                                Direction option2 = none;
                                switch(getDirection())
                                {
                                    case up:
                                    case down:
                                        option1 = left;
                                        option2 = right;
                                        break;
                                    case left:
                                    case right:
                                        option1 = up;
                                        option2 = down;
                                    case none:
                                    default:
                                        break;
                                }
                                
                                if (newDir == 1 && sw->protesterTryToMove(x, y, option1))
                                {
                                    moveInDirection(option1);
                                    resetPerpendicularTicks(option1);
                                    return;
                                }
                                if (sw->protesterTryToMove(x, y, option2))
                                {
                                    moveInDirection(option2);
                                    resetPerpendicularTicks(option1);
                                    return;
                                }
                                else if(newDir == 2 && sw->protesterTryToMove(x, y, option1))
                                {
                                    moveInDirection(option1);
                                    resetPerpendicularTicks(option1);
                                }
                                else
                                {
                                    setNumSquaresToMove(0);
                                    setPerpendicularTicks(0);
                                }
                            }
                        }
                        else
                        {
                            setNumSquaresToMove(0);
                        }
                    }
                }
                incWaitingTicksTracking();
                incRestingTicksTracking();
            }
        }
    }
}

/*-----------------------------------------------------------------------
                           PICKABLE IMPLEMENTATION
 -----------------------------------------------------------------------*/

PickableObject::PickableObject(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, bool stableState = true, int time = 0):Actor(imageID, startX, startY, dir, size, depth, studentWorld){}

bool PickableObject::isPickedUp()
{
    if (getStudentWorld()->checkTunnelMan(getX(), getY(), 3.0))
    {
        setAlive(false);
        setVisible(false);
        return true;
    }
    return false;
}

void PickableObject::tick()
{
    if(getTime() > 0)
    {
        decTime();
    }
    else
    {
        setVisible(false);
        setAlive(false);
    }
}

/*-----------------------------------------------------------------------
                            BARREL IMPLEMENTATION
 -----------------------------------------------------------------------*/


Barrel::Barrel(int startX, int startY, StudentWorld* studentWorld): PickableObject(TID_BARREL, startX, startY, right, 1.0, 2, studentWorld, true, 1)
{
    setVisible(false);
}

void Barrel::doSomething()
{
    if (isAlive())
    {
        StudentWorld* sw = getStudentWorld();
        if (isPickedUp())
            sw->oilPickedUp();
        if (sw->checkTunnelMan(getX(), getY(), 4.0))
        {
            setVisible(true);
            return;
        }
    }
    return;
}

/*-----------------------------------------------------------------------
                           SONARKIT IMPLEMENTATION
 -----------------------------------------------------------------------*/

SonarKit::SonarKit(int startX, int startY, StudentWorld* studentWorld):PickableObject(TID_SONAR, startX, startY, right, 1.0, 2.0, studentWorld, false)
{
    const int level = getStudentWorld()->getLevel();
    setTime(max(100, 300 - 10 * level));
}

void SonarKit::doSomething()
{
    if (isAlive())
    {
        if (isPickedUp())
        {
            getStudentWorld()->sonarPickedUp();
        }
        
        tick();
    }
    return;
}

/*-----------------------------------------------------------------------
                           WATERPOOL IMPLEMENTATION
 -----------------------------------------------------------------------*/

WaterPool::WaterPool(int startX, int startY, StudentWorld* studentWorld):PickableObject(TID_WATER_POOL, startX, startY, right, 1.0, 2.0, studentWorld, false)
{
    const int level = getStudentWorld()->getLevel();
    setTime(max(100, 300 - 10 * level));
}

void WaterPool::doSomething()
{
    if (isAlive())
    {
        if (isPickedUp())
        {
            getStudentWorld()->waterPoolPickedUp();
        }
        
        tick();
    }
    return;
}

/*-----------------------------------------------------------------------
                          GOLDNUGGET IMPLEMENTATION
 -----------------------------------------------------------------------*/

GoldNugget::GoldNugget(int startX, int startY, StudentWorld* studentWorld, bool stableState):PickableObject(TID_GOLD, startX, startY, right, 1.0, 2.0, studentWorld, false)
{
    const int level = getStudentWorld()->getLevel();
    setTime(max(100, 300 - 10 * level));
    if(stableState)
        setVisible(false);
    else
        setVisible(true);
    setStable(stableState);
}

void GoldNugget::doSomething()
{
    if (isAlive())
    {
        StudentWorld* sw = getStudentWorld();
        if (isStable())
        {
            if(isPickedUp())
            {
                sw->goldPickedUp();
            }
            if (sw->checkTunnelMan(getX(), getY(), 4.0))
            {
                setVisible(true);
                return;
            }
        }
        else
        {
            if (sw->goldBribed(getX(), getY()))
            {
                setVisible(false);
                setAlive(false);
            }
            tick();
        }
    }
}

/*-----------------------------------------------------------------------
                        DAMAGEOBJECT IMPLEMENTATION
 -----------------------------------------------------------------------*/

DamageObject::DamageObject(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, int move):Actor(imageID, startX, startY, dir, size, depth, studentWorld), m_move(move){}

/*-----------------------------------------------------------------------
                           BOULDER IMPLEMENTATION
 -----------------------------------------------------------------------*/

Boulder::Boulder(int startX, int startY, StudentWorld* studentWorld):DamageObject(TID_BOULDER, startX, startY, down, 1.0, 1, studentWorld, startY), m_time(30), m_state(stable){}

void Boulder::doSomething()
{
    if (isAlive())
    {
        StudentWorld* sw = getStudentWorld();
        if(m_state == stable)
        {
            if(!sw->checkSupport(getX(), getY()))
            {
                m_state = waiting;
                return;
            }
        }
        else if(m_state == waiting)
        {
            if (m_time > 0)
                m_time--;
            else
            {
                m_state = falling;
                sw->playSound(SOUND_FALLING_ROCK);
                return;
            }
        }
        else
        {
            if(!getStudentWorld()->isBonked(getX(), getY()) && getMove() >= 0)
            {
                moveTo(getX(), getY() - 1);
                decMove();
            }
            else
            {
                setAlive(false);
                setVisible(false);
            }
        }
    }
}

/*-----------------------------------------------------------------------
                            SQUIRT IMPLEMENTATION
 -----------------------------------------------------------------------*/

Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* studentWorld):DamageObject(TID_WATER_SPURT, startX, startY, dir, 1.0, 1, studentWorld, 5){}

void Squirt::doSomething()
{
    if(isAlive())
    {
        if (getMove() > 0)
        {
            if(getStudentWorld()->isSquirted(getX(), getY()) || getX() > 60 || getX() < 0 || getY() > 60 || getY() < 0)
            {
                setVisible(false);
                setAlive(false);
            }
            else
            {
                decMove();
                switch(getDirection())
                {
                    case up:
                        moveTo(getX(), getY() + 1);
                        break;
                    case down:
                        moveTo(getX(), getY() - 1);
                        break;
                    case left:
                        moveTo(getX() - 1, getY());
                        break;
                    case right:
                        moveTo(getX() + 1, getY());
                        break;
                    case none:
                    default:
                        break;
                }
            }
        }
        else
        {
            setVisible(false);
            setAlive(false);
        }
    }
}
