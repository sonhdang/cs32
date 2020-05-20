//
//  Arena.h
//  Project1
//
//  Created by Son Dang on 6/25/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef Arena_h
#define Arena_h
#include "globals.h"
#include <string>
#include "Before.h"

class Robot;
class Player;


class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    void    display(std::string msg) const;
    Before& whatWasBefore();
    
    // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    void   damageRobotAt(int r, int c);
    bool   moveRobots();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
    Before  m_before;
};

#endif /* Arena_h */
