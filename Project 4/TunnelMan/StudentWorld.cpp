#include "StudentWorld.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

/*-----------------------------------------------------------------------
                          PRIVATE HELPER FUNCTIONS
 -----------------------------------------------------------------------*/

void StudentWorld::setDisplayText()
{
    ostringstream oss;
    oss << "Lvl: " << setw(2) << getLevel() << "  Lives: " <<  setw(1) << getLives() << setw(3) << "  Hlth: " << (m_player->getHealth()) * 10 << "%  Wtr: " << setw(2) << m_player->getWater() << " Gld: " << m_player->getGold() << " Oil Left: " << m_numOil << "  Sonar: " << m_player->getSonar();
    oss.fill('0');
    oss << "  Scr: " << setw(6) << getScore();
    
    setGameStatText(oss.str());
}

bool StudentWorld::isInRange(int x, int y, double dist)
{
    if (m_actors.size() == 0)
        return false;
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (checkDistance(x, y, (*it)->getX(), (*it)->getY(), dist))
            return true;
    }
    return false;
}

bool StudentWorld::isEmptySpace(int x, int y)
{
    for(int i = y; i < y + 4; i++)
    {
        for (int j = x; j < x + 4; j++)
        {
            if (m_earths[i][j] != nullptr)
            {
                return false;
            }
        }
    }
    
    
    return true;
}

bool StudentWorld::checkDistance(int x1, int y1, int x2, int y2, double distance)
{
    int differenceX = x1 - x2;
    int differenceY = y1 - y2;
    double actualDistance = sqrt(differenceX * differenceX + differenceY * differenceY);
    return actualDistance <= distance;
}

void StudentWorld::addBoulder(int num)
{
    for (int i = 0; i < num; i++)      //Add Boulders
    {
        bool flip = true;
        int x = -1;
        int y = -1;
        do
        {
            if (flip)
            {
                x = random(0, 26);
                flip = false;
            }
            else
            {
                x = random(34, 60);
                flip = true;
            }
            y = random(20, 56);
        }
        while(isInRange(x,y, 3.0));
        
        m_actors.push_back(new Boulder(x, y, this));
        
        for (int i = y; i < y + 4; i++)
        {
            for (int j = x; j < x + 4; j++)
            {
                delete m_earths[i][j];
                m_earths[i][j] = nullptr;
            }
        }
    }
}

void StudentWorld::addGoodies(int numGold, int numBarrel)
{
    int num = numGold + numBarrel;
    for (int i = 0; i < num; i++)      //Add Boulders
    {
        bool flip = true;
        int x = -1;
        int y = -1;
        do
        {
            if (flip)
            {
                x = random(0, 26);
                flip = false;
            }
            else
            {
                x = random(34, 60);
                flip = true;
            }
            y = random(0, 56);
        }
        while(isInRange(x, y, 7.0));
        if (numGold > 0)
        {
            m_actors.push_back(new GoldNugget(x, y, this, true));
            numGold--;
        }
        else
        {
            m_actors.push_back(new Barrel(x, y, this));
            numBarrel--;
        }
    }
}

void StudentWorld::addSonarKit()
{
    m_actors.push_back(new SonarKit(0, 60, this));
}

void StudentWorld::addWaterPool()
{
    int x = -1;
    int y = -1;
    do
    {
        x = random(0,60);
        y = random(0,60);
    }
    while(!isEmptySpace(x, y));
    m_actors.push_back(new WaterPool(x, y, this));
}

/*-----------------------------------------------------------------------
                              PUBLIC FUNCTIONS
 -----------------------------------------------------------------------*/

int StudentWorld::random(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

void StudentWorld::moveTunnelMan()
{
    int key;
    if (getKey(key))            //getting the key from the user
    {
        int x = m_player->getX();                         //current x coordinate of the player
        int y = m_player->getY();                         //current y coordinate of the player
        switch(key)
        {
            case KEY_PRESS_UP:
                if (m_player->getDirection() != Actor::up)       //change the direction if tunnelman is not in the direction
                    m_player->setDirection(Actor::up);
                else
                {
                    if (y < 60)                 //move if the tunnel man is already in the direction
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if(m_earths[y + 4][x + i] != nullptr)
                            {
                                delete m_earths[y + 4][x + i];
                                m_earths[y + 4][x + i] = nullptr;
                            }
                        }
                        m_player->moveTo(x, y + 1);
                    }
                }
                break;
            case KEY_PRESS_DOWN:
                if (m_player->getDirection() != Actor::down)
                    m_player->setDirection(Actor::down);
                else
                {
                    if (y > 0)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if(m_earths[y - 1][x + i] != nullptr)
                            {
                                delete m_earths[y - 1][x + i];
                                m_earths[y - 1][x + i] = nullptr;
                            }
                        }
                        m_player->moveTo(x, y - 1);
                    }
                }
                break;
            case KEY_PRESS_LEFT:
                if (m_player->getDirection() != Actor::left)
                    m_player->setDirection(Actor::left);
                else
                {
                    if (x > 0)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if(m_earths[y + i][x - 1] != nullptr)
                            {
                                delete m_earths[y + i][x - 1];
                                m_earths[y + i][x - 1] = nullptr;
                            }
                        }
                        m_player->moveTo(x - 1, y);
                    }
                }
                break;
            case KEY_PRESS_RIGHT:
                if (m_player->getDirection() != Actor::right)
                    m_player->setDirection(Actor::right);
                else
                {
                    if (x < 60)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if(m_earths[y + i][x + 4] != nullptr)
                            {
                                delete m_earths[y + i][x + 4];
                                m_earths[y + i][x + 4] = nullptr;
                            }
                        }
                        m_player->moveTo(x + 1, y);
                    }
                }
                break;
            case KEY_PRESS_TAB:
                if (m_player->getGold() > 0)
                {
                    m_actors.push_back(new GoldNugget(x, y, this, false));
                    m_player->setGold(m_player->getGold() - 1);
                }
                break;
            case KEY_PRESS_SPACE:
                if(m_player->getWater() <= 0)
                    break;
                else
                {
                    int squirtX = m_player->getX();
                    int squirtY = m_player->getY();
                    switch(m_player->getDirection())
                    {
                        case Actor::up:
                            squirtY += 4;
                            m_player->setWater(m_player->getWater() - 1);
                            break;
                        case Actor::down:
                            squirtY -= 4;
                            m_player->setWater(m_player->getWater() - 1);
                            break;
                        case Actor::left:
                            squirtX -= 4;
                            m_player->setWater(m_player->getWater() - 1);
                            break;
                        case Actor::right:
                            squirtX += 4;
                            m_player->setWater(m_player->getWater() - 1);
                            break;
                        case Actor::none:
                        default:
                            
                            break;
                    }
                    m_actors.push_back(new Squirt(squirtX, squirtY, m_player->getDirection(), this));
                    playSound(SOUND_PLAYER_SQUIRT);
                    
                    break;
                }
            case KEY_PRESS_ESCAPE:              //when the user wants to restart the level
                decLives();
                m_player->setAlive(false);
                break;
        }
    }
}

bool StudentWorld::checkTunnelMan(int x, int y, double distance)
{
    int differenceX = x - m_player->getX();
    int differenceY = y - m_player->getY();
    double actualDistance = sqrt(differenceX * differenceX + differenceY * differenceY);
    return actualDistance <= distance;
}

bool StudentWorld::isFacingTunnelMan(int x, int y, Actor::Direction dir)
{
    switch(dir)
    {
        case Actor::up:
            return m_player->getY() - y > 0;
        case Actor::down:
            return y - m_player->getY() > 0;
        case Actor::left:
            return x - m_player->getX() > 0;
        case Actor::right:
            return m_player->getX() - x > 0;
        case Actor::none:
        default:
            return false;
    }
}

void StudentWorld::yellAtTunnelMan()
{
    playSound(SOUND_PROTESTER_YELL);
    m_player->doDamage(2);
}

bool StudentWorld::clearPathToTunnelMan(int x, int y, Actor::Direction& dir)
{
    int playerX = m_player->getX();
    int playerY = m_player->getY();
    if (playerX == x)
    {
        for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        {
            if ((*it)->isBoulder() && abs(x - (*it)->getX()) < 4)
            {
                return false;
            }
        }
        
        int beginning;
        int end;
        
        if (y > playerY)
        {
            dir = Actor::down;
            beginning = playerY;
            end = y;
        }
        else
        {
            dir = Actor::up;
            beginning = y;
            end = playerY;
        }
        
        for (int i = beginning; i < end; i++)
        {
            for (int j = x; j < x + 4; j++)
            {
                if(m_earths[i][j] != nullptr)
                {
                    dir = Actor::none;
                    return false;
                }
            }
        }
        
        return true;
    }
    else if(m_player->getY() == y)
    {
        for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        {
            if ((*it)->isBoulder() && abs(y - (*it)->getY()) < 4)
            {
                return false;
            }
        }
        
        int beginning;
        int end;
        
        if (x > playerX)
        {
            dir = Actor::left;
            beginning = playerX;
            end = x;
        }
        else
        {
            dir = Actor::right;
            beginning = x;
            end = playerX;
        }
        
        for (int i = y; i < y + 4; i++)
        {
            for (int j = beginning; j < end; j++)
            {
                if(m_earths[i][j] != nullptr)
                {
                    dir = Actor::none;
                    return false;
                }
            }
        }
        
        return true;
    }
    return false;
}


void StudentWorld::oilPickedUp()
{
    playSound(SOUND_FOUND_OIL);
    increaseScore(1000);
    m_numOil--;
}

void StudentWorld::waterPoolPickedUp()
{
    playSound(SOUND_GOT_GOODIE);
    increaseScore(100);
    m_player->setWater(m_player->getWater() + 5);
}

void StudentWorld::sonarPickedUp()
{
    playSound(SOUND_GOT_GOODIE);
    increaseScore(75);
    m_player->setSonar(m_player->getSonar() + 1);
}

void StudentWorld::goldPickedUp()
{
    playSound(SOUND_GOT_GOODIE);
    increaseScore(10);
    m_player->setGold(m_player->getGold() + 1);
}

bool StudentWorld::goldBribed(int x, int y)
{
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isProtester())
        {
            if (checkDistance(x, y, (*it)->getX(), (*it)->getY(), 3.0))
            {
                playSound(SOUND_PROTESTER_FOUND_GOLD);
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::checkSupport(int x, int y)
{
    for (int i = x; i < x + 4; i++)
    {
        if(m_earths[y - 1][i] != nullptr)
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isBonked(int x, int y)
{
    for (int i = x; i < x + 4; i++)
    {
        if(m_earths[y - 1][i] != nullptr)
        {
            return true;
        }
    }
    
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isBonkable())
        {
            if (checkDistance(x, y, (*it)->getX(), (*it)->getY(), 3.0))
            {
                (*it)->doDamage(100);
                playSound(SOUND_PROTESTER_GIVE_UP);
            }
        }
    }
    
    if (checkTunnelMan(x, y, 3.0))
    {
        decLives();
        playSound(SOUND_PLAYER_GIVE_UP);
        m_player->setVisible(false);
        m_player->setAlive(false);
        return true;
    }
    return false;
}

bool StudentWorld::isSquirted(int x, int y)
{
    if(!isEmptySpace(x, y))
        return true;
    
    
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (((*it)->isProtester() || (*it)->isBoulder()) && checkDistance(x, y, (*it)->getX(), (*it)->getY(), 3.0))
        {
            if((*it)->isProtester())
            {
                (*it)->doDamage(2);
                playSound(SOUND_PROTESTER_ANNOYED);
            }
            return true;
        }
    }
    return false;
}

bool StudentWorld::protesterTryToMove(int x, int y, Actor::Direction dir)
{
    int nextX = x;
    int nextY = y;
    switch(dir)
    {
        case Actor::up:
            if (y >= 60)
                return false;
            for (int i = 0; i < 4; i++)
            {
                if(m_earths[y + 4][x + i] != nullptr)
                {
                    return false;
                }
            }
            nextY++;
            break;
            
        case Actor::down:
            if (y <= 0)
                return false;
            for (int i = 0; i < 4; i++)
            {
                if(m_earths[y - 1][x + i] != nullptr)
                {
                    return false;
                }
            }
            nextY--;
            break;
        case Actor::left:
            if (x <= 0)
                return false;
            for (int i = 0; i < 4; i++)
            {
                if(m_earths[y + i][x - 1] != nullptr)
                {
                    return false;
                }
            }
            nextX--;
            break;
        case Actor::right:
            if (x >= 60)
                return false;
            for (int i = 0; i < 4; i++)
            {
                if(m_earths[y + i][x + 4] != nullptr)
                {
                    return false;
                }
            }
            nextX++;
            break;
        case Actor::none:
        default:
            return false;
    }
    
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isBoulder() && checkDistance((*it)->getX(), (*it)->getY(), nextX, nextY, 3.0))
        {
            return false;
        }
    }
    return true;
}

/*-----------------------------------------------------------------------
                          STUDENTWORLD FUNCTIONS
 -----------------------------------------------------------------------*/

int StudentWorld::init()
{
    srand(time(NULL));
    const int level = getLevel();
    m_numOil = min(2 + level, 21);              //calculate the number of oil each level
    m_numGold = max(5 - level, 2);              //calculate the number of gold each level
    m_numBoulder = min(level / 2 + 2, 9);       //calculate the number of boulder each level
    m_goodieChance = level * 25 + 300;
    
    m_earths = new Earth**[64];                 //initializing the 2D array of Earth pointers to nullptr
    for (int i = 0; i < 64; i++)
    {
        m_earths[i] = new Earth*[64];
        
        for(int j = 0; j < 64; j++)
            m_earths[i][j] = nullptr;
    }
    
    m_player = new TunnelMan(this);            //fill the field with earth objects
    
    for (int r = 0; r < VIEW_HEIGHT - 4; r++)
    {
        for (int c = 0; c < 30; c++)
        {
            m_earths[r][c] = new Earth(c, r, this);
            m_earths[r][c + 34] = new Earth(c + 34, r, this);
        }
    }
    
    for (int r = 0; r < 4; r++)                 //fill the 4x4 square in the middle bottom of the field with
    {                                           //earth objects
        for(int c = 30; c < 34; c++)
            m_earths[r][c] = new Earth(c, r, this);
    }
    
    addBoulder(m_numBoulder);
    addGoodies(m_numGold, m_numOil);
    m_actors.push_back(new RegularProtester(this));
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();                           //display the status text
    
    m_player->doSomething();                    //tell the TunnelMan to do something
    
    for (list<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
    {
        if ((*i)->isAlive())                    //tell all the graph objects to do something
            (*i)->doSomething();
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        if (m_numOil == 0)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    if (rand() % m_goodieChance < 1)
    {
        if(rand() % 5 < 1)
        {
            addSonarKit();
        }
        else
        {
            
            cout << "addWaterPool()" << endl;
            addWaterPool();
            cout << "WaterPool added" << endl;
        }
    }
    
    list<Actor*>::iterator i = m_actors.begin();
    while (i != m_actors.end())                 //delete dead graph objects after its turn to do something
    {
        if(!(*i)->isAlive())
        {
            delete (*i);
            (*i) = nullptr;
            i = m_actors.erase(i);
        }
        else
            i++;
    }
    
    if (!m_player->isAlive())
        return GWSTATUS_PLAYER_DIED;
    
    if (m_numOil == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;                            //delete the TunnelMan
    
    for (list<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++)
    {
        delete *i;                              //delete all the graph objects
    }
    
    m_actors.clear();
    
    for (int r = 0; r < VIEW_HEIGHT; r++)       //delete all earth objects
    {
        for (int c = 0; c < VIEW_WIDTH; c++)
        {
            if(m_earths[r][c] != nullptr)
            {
                delete m_earths[r][c];
                m_earths[r][c] = nullptr;
            }
        }
    }
    
    m_numOil = 0;                               //reset all the integral types to NULL
    m_numGold = 0;
    m_numBoulder = 0;
}
