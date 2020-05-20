//
//  Robot.h
//  Project1
//
//  Created by Son Dang on 6/25/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef Robot_h
#define Robot_h

class Arena;

class Robot
{
public:
    // Constructor
    Robot(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool takeDamageAndLive();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
};

#endif /* Robot_h */
