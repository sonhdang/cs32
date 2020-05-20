//
//  main.cpp
//  Project1
//
//  Created by Son Dang on 6/25/19.
//  Copyright © 2019 Son Dang. All rights reserved.
// zion_solution.cpp

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;

int main()
{
    // Initialize the random number generator.  (You don't need to
    // understand how this works.)
    srand(static_cast<unsigned int>(time(0)));
    
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(15, 18, 80);
    
    // Play the game
    g.play();
}
