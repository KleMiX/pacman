//
//  CEnemyClyde.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEnemyClyde.h"
#include <math.h>

CEnemyClyde::CEnemyClyde(CMap *map) : CEnemy(map)
{
    // loading Blinky sprites
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sprintf(filename, "clyde_%c%d.png", directions[i], j + 1);
            CEntity::loadSprite(filename, i, j);
        }
    }
    
    // setting starting position and scatter phase corner individually for every ghost
    _startX = 15;
    _startY = 17;
    _scatterX = 0;
    _scatterY = 34;
    _chaseX = 0;
    _chaseY = 0;
    
    Reset(); // resets values and starts movement
}

void CEnemyClyde::setPlayerPositionAndDirection(int x, int y, Direction direction)
{
    // getting euclidean distance to pacman
    int x1 = x - getXposition();
    int y1 = y - getYposition();
    
    float euclideanDist = sqrtf(x1 * x1 + y1 * y1); // pithagoras!
    
    if (euclideanDist < 8) { // too near, run away
        _chaseX = _scatterX;
        _chaseY = _scatterY;
    }
    else { // too far, get back, chase like Blinky
        _chaseX = x;
        _chaseY = y;
    }
}