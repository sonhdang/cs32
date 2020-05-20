//
//  Game.h
//  Project1
//
//  Created by Son Dang on 6/25/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef Game_h
#define Game_h

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};


#endif /* Game_h */
