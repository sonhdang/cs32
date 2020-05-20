//
//  Player.h
//  Project1
//
//  Created by Son Dang on 6/25/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include "Arena.h"


class Arena;

class Player
{
public:
    // Constructor
    Player(Arena *ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
    
    // Mutators
    std::string takeComputerChosenTurn();
    void   stand();
    void   move(int dir);
    bool   shoot(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
    
    int    computeDanger(int r, int c) const;
};

#endif /* Player_h */
